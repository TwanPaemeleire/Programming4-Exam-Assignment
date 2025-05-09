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
private:
	void SoundThreadLoop(const std::stop_token& stopToken);

	enum class SoundRequestType
	{
		LoadSound,
		LoadMusic,
		PlaySound,
		PlayMusic,
		StopSound,
		StopMusic
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

	void LoadMusic(const SoundData& soundData);
	void LoadSound(const SoundData& soundData);
	void PlayMusic(const SoundData& soundData);
	void PlaySound(const SoundData& soundData);

	// Define a custom deleter so we don't have to worry about having to work with raw pointers
	struct MixMusicDeleter
	{
		void operator()(Mix_Music* music) const
		{
			Mix_FreeMusic(music);
		}
	};
	std::unordered_map<SoundId, std::unique_ptr<Mix_Music, MixMusicDeleter>> m_LoadedMusic;

	// Define a custom deleter so we don't have to worry about having to work with raw pointers
	struct MixChunkDeleter
	{
		void operator()(Mix_Chunk* chunk) const
		{
			Mix_FreeChunk(chunk);
		}
	};
	std::unordered_map<SoundId, std::unique_ptr<Mix_Chunk, MixChunkDeleter>> m_LoadedChunks;
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
		}
	}
}

void Twengine::SDLSoundSystem::SDLSoundSystemImpl::LoadMusic(const SoundData& soundData)
{
	if (m_LoadedMusic.find(soundData.id) == m_LoadedMusic.end())
		m_LoadedMusic.insert(std::pair(soundData.id, Mix_LoadMUS(soundData.file.c_str())));
}

void Twengine::SDLSoundSystem::SDLSoundSystemImpl::LoadSound(const SoundData& soundData)
{
	if (m_LoadedChunks.find(soundData.id) == m_LoadedChunks.end())
		m_LoadedChunks.insert(std::pair(soundData.id, Mix_LoadWAV(soundData.file.c_str())));
}

void Twengine::SDLSoundSystem::SDLSoundSystemImpl::PlayMusic(const SoundData& soundData)
{
	Mix_Music* music = m_LoadedMusic.at(soundData.id).get();
	Mix_VolumeMusic(static_cast<int>(soundData.volume * MIX_MAX_VOLUME));
	Mix_PlayMusic(music, -1);
}

void Twengine::SDLSoundSystem::SDLSoundSystemImpl::PlaySound(const SoundData& soundData)
{
	Mix_Chunk* chunk = m_LoadedChunks.at(soundData.id).get();
	Mix_VolumeChunk(chunk, static_cast<int>(soundData.volume * MIX_MAX_VOLUME));
	int channel = Mix_PlayChannel(-1, chunk, 0);
	Mix_SetPosition(channel, 90, 50);
}