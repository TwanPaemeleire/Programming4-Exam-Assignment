#include "SDLSoundSystem.h"
#include <iostream>
#include "SDL.h"
#include <unordered_map>
#include "SDL_mixer.h"
#include <queue>
#include <condition_variable>
#include <thread>

class Twengine::SDLSoundSystem::SDLSoundSystemImpl
{
public:
	SDLSoundSystemImpl();
	~SDLSoundSystemImpl();
	SDLSoundSystemImpl(const SDLSoundSystemImpl& other) = delete;
	SDLSoundSystemImpl(SDLSoundSystemImpl&& other) = delete;
	SDLSoundSystemImpl& operator=(const SDLSoundSystemImpl& other) = delete;
	SDLSoundSystemImpl& operator=(SDLSoundSystemImpl&& other) = delete;
	void RequestLoadMusic(const std::string& file, SoundId id);
	void RequestLoadSound(const std::string& file, SoundId id);
	void RequestPlayMusic(SoundId id, const float volume);
	void RequestPlaySound(SoundId id, const float volume);
	void RequestMuteAllSound();
	void RequestUnMuteAllSound();
	void RequestStopAllSound();
	bool IsMuted() const;
private:
	void SoundThreadLoop(const std::stop_token& stopToken);

	enum class SoundRequestType
	{
		LoadSound,
		LoadMusic,
		PlaySound,
		PlayMusic,
		MuteAll,
		UnmuteAll,
		StopAll
	};

	struct SoundData
	{
		SoundRequestType type;
		SoundId id;
		float volume;
		std::string file;
	};
	std::jthread m_SoundThread;
	std::queue<SoundData> m_SoundQueue;
	std::mutex m_QueueMutex;
	std::condition_variable m_Condition;
	bool m_IsMuted{ false };

	void LoadMusic(const SoundData& soundData);
	void LoadSound(const SoundData& soundData);
	void PlayMusic(const SoundData& soundData);
	void PlaySound(const SoundData& soundData);
	void MuteAll();
	void UnMuteAll();
	void StopAll();

	// Define a custom deleter so we don't have to worry about having to work with raw pointers
	struct MixMusicDeleter
	{
		void operator()(Mix_Music* music) const
		{
			Mix_FreeMusic(music);
		}
	};
	std::unordered_map<SoundId, std::unique_ptr<Mix_Music, MixMusicDeleter>> m_LoadedMusic;
	float m_LastMusicVolume{};

	// Define a custom deleter so we don't have to worry about having to work with raw pointers
	struct MixChunkDeleter
	{
		void operator()(Mix_Chunk* chunk) const
		{
			Mix_FreeChunk(chunk);
		}
	};
	std::unordered_map<SoundId, std::unique_ptr<Mix_Chunk, MixChunkDeleter>> m_LoadedChunks;

	struct ChunkSettings
	{
		int channel{};
		float volume{};
	};
	std::unordered_map<SoundId, ChunkSettings> m_ChunkSettings;
};


Twengine::SDLSoundSystem::SDLSoundSystem()
	: m_pImplSDLSoundSystem{new SDLSoundSystemImpl()}
{
}

Twengine::SDLSoundSystem::~SDLSoundSystem()
{
	delete m_pImplSDLSoundSystem;
}

void Twengine::SDLSoundSystem::RequestLoadMusic(const std::string& file, SoundId id)
{
	m_pImplSDLSoundSystem->RequestLoadMusic(file, id);
}

void Twengine::SDLSoundSystem::RequestLoadSound(const std::string& file, SoundId id)
{
	m_pImplSDLSoundSystem->RequestLoadSound(file, id);
}

void Twengine::SDLSoundSystem::RequestPlayMusic(SoundId id, const float volume)
{
	m_pImplSDLSoundSystem->RequestPlayMusic(id, volume);
}

void Twengine::SDLSoundSystem::RequestPlaySound(SoundId id, const float volume)
{
	m_pImplSDLSoundSystem->RequestPlaySound(id, volume);
}

void Twengine::SDLSoundSystem::RequestMuteAllSound()
{
	m_pImplSDLSoundSystem->RequestMuteAllSound();
}

void Twengine::SDLSoundSystem::RequestUnMuteAllSound()
{
	m_pImplSDLSoundSystem->RequestUnMuteAllSound();
}

void Twengine::SDLSoundSystem::RequestStopAllSound()
{
	m_pImplSDLSoundSystem->RequestStopAllSound();
}

bool Twengine::SDLSoundSystem::IsMuted() const
{
	return m_pImplSDLSoundSystem->IsMuted();
}

Twengine::SDLSoundSystem::SDLSoundSystemImpl::SDLSoundSystemImpl()
{
	SDL_Init(SDL_INIT_AUDIO);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	m_SoundThread = std::jthread([this](const std::stop_token& stopToken)
		{
			SoundThreadLoop(stopToken);
		});
}

Twengine::SDLSoundSystem::SDLSoundSystemImpl::~SDLSoundSystemImpl()
{
	// Make sure the thread isn't stuck in waiting infinitely
	m_Condition.notify_one();
	m_SoundThread.request_stop();

	Mix_CloseAudio();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

void Twengine::SDLSoundSystem::SDLSoundSystemImpl::RequestLoadMusic(const std::string& file, SoundId id)
{
	std::lock_guard<std::mutex> lock(m_QueueMutex);
	m_SoundQueue.push({ SoundRequestType::LoadMusic, id, 0, file });
	m_Condition.notify_one();
}

void Twengine::SDLSoundSystem::SDLSoundSystemImpl::RequestLoadSound(const std::string& file, SoundId id)
{
	std::lock_guard<std::mutex> lock(m_QueueMutex);
	m_SoundQueue.push({ SoundRequestType::LoadSound, id, 0, file });
	m_Condition.notify_one();
}

void Twengine::SDLSoundSystem::SDLSoundSystemImpl::RequestPlayMusic(SoundId id, const float volume)
{
	std::lock_guard<std::mutex> lock(m_QueueMutex);
	m_SoundQueue.push({ SoundRequestType::PlayMusic, id, volume });
	m_Condition.notify_one();
}

void Twengine::SDLSoundSystem::SDLSoundSystemImpl::RequestPlaySound(SoundId id, const float volume)
{
	std::lock_guard<std::mutex> lock(m_QueueMutex);
	m_SoundQueue.push({ SoundRequestType::PlaySound, id, volume });
	m_Condition.notify_one();
}

void Twengine::SDLSoundSystem::SDLSoundSystemImpl::RequestMuteAllSound()
{
	std::lock_guard<std::mutex> lock(m_QueueMutex);
	m_SoundQueue.push({ SoundRequestType::MuteAll, 0, 0 });
	m_Condition.notify_one();
}

void Twengine::SDLSoundSystem::SDLSoundSystemImpl::RequestUnMuteAllSound()
{
	std::lock_guard<std::mutex> lock(m_QueueMutex);
	m_SoundQueue.push({ SoundRequestType::UnmuteAll, 0, 0 });
	m_Condition.notify_one();
}

void Twengine::SDLSoundSystem::SDLSoundSystemImpl::RequestStopAllSound()
{
	std::lock_guard<std::mutex> lock(m_QueueMutex);
	m_SoundQueue.push({ SoundRequestType::StopAll, 0, 0 });
	m_Condition.notify_one();
}

bool Twengine::SDLSoundSystem::SDLSoundSystemImpl::IsMuted() const
{
	return m_IsMuted;
}

void Twengine::SDLSoundSystem::SDLSoundSystemImpl::SoundThreadLoop(const std::stop_token& stopToken)
{
	while (!stopToken.stop_requested())
	{
		std::unique_lock<std::mutex> lock(m_QueueMutex);

		// Wait until something gets added to queue or we request for the thread to stop
		m_Condition.wait(lock, [this, &stopToken] { return !m_SoundQueue.empty() || stopToken.stop_requested(); });

		if (stopToken.stop_requested()) break;

		SoundData soundData = m_SoundQueue.front();
		m_SoundQueue.pop();

		lock.unlock();

		switch (soundData.type)
		{
			case SoundRequestType::LoadMusic:
			{
				LoadMusic(soundData);
				break;
			}
			case SoundRequestType::LoadSound:
			{
				LoadSound(soundData);
				break;
			}
			case SoundRequestType::PlayMusic:
			{
				PlayMusic(soundData);
				break;
			}
			case SoundRequestType::PlaySound:
			{
				PlaySound(soundData);
				break;
			}
			case SoundRequestType::MuteAll:
			{
				MuteAll();
				break;
			}
			case SoundRequestType::UnmuteAll:
			{
				UnMuteAll();
				break;
			}
			case SoundRequestType::StopAll:
			{
				StopAll();
				break;
			}
		}
	}
}

void Twengine::SDLSoundSystem::SDLSoundSystemImpl::LoadMusic(const SoundData& soundData)
{
	if (m_LoadedMusic.find(soundData.id) == m_LoadedMusic.end())
	{
		m_LoadedMusic.insert(std::pair(soundData.id, Mix_LoadMUS(soundData.file.c_str())));
	}
}

void Twengine::SDLSoundSystem::SDLSoundSystemImpl::LoadSound(const SoundData& soundData)
{
	if (m_LoadedChunks.find(soundData.id) == m_LoadedChunks.end())
	{
		m_LoadedChunks.insert(std::pair(soundData.id, Mix_LoadWAV(soundData.file.c_str())));
		m_ChunkSettings.insert(std::pair(soundData.id, ChunkSettings{}));
	}
}

void Twengine::SDLSoundSystem::SDLSoundSystemImpl::PlayMusic(const SoundData& soundData)
{
	Mix_Music* music = m_LoadedMusic.at(soundData.id).get();
	if (!m_IsMuted)
	{
		Mix_VolumeMusic(static_cast<int>(soundData.volume * MIX_MAX_VOLUME));
		m_LastMusicVolume = soundData.volume;
	}
	else Mix_VolumeMusic(0);
	Mix_PlayMusic(music, -1);
}

void Twengine::SDLSoundSystem::SDLSoundSystemImpl::PlaySound(const SoundData& soundData)
{
	Mix_Chunk* chunk = m_LoadedChunks.at(soundData.id).get();
	if(!m_IsMuted) Mix_VolumeChunk(chunk, static_cast<int>(soundData.volume * MIX_MAX_VOLUME));
	else Mix_VolumeChunk(chunk, 0);
	int channel = Mix_PlayChannel(-1, chunk, 0);
	m_ChunkSettings.insert(std::pair(soundData.id, ChunkSettings{channel, soundData.volume}));
}

void Twengine::SDLSoundSystem::SDLSoundSystemImpl::MuteAll()
{
	m_IsMuted = true;
	for (const auto& [id, chunk] : m_LoadedChunks)
	{
		if (chunk)
		{
			Mix_VolumeChunk(chunk.get(), 0);
		}
	}
	Mix_VolumeMusic(0);
}

void Twengine::SDLSoundSystem::SDLSoundSystemImpl::UnMuteAll()
{
	m_IsMuted = false;
	for (const auto& [id, chunkSetting] : m_ChunkSettings)
	{
		auto it = m_LoadedChunks.find(id);
		if (it != m_LoadedChunks.end())
		{
			Mix_VolumeChunk(it->second.get(), static_cast<int>(chunkSetting.volume * MIX_MAX_VOLUME));
		}
	}
	Mix_VolumeMusic(static_cast<int>(m_LastMusicVolume * MIX_MAX_VOLUME));
}

void Twengine::SDLSoundSystem::SDLSoundSystemImpl::StopAll()
{
	Mix_HaltChannel(-1);
	Mix_HaltMusic();
}
