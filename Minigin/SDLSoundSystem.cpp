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
	virtual void RequestPlayMusic(const std::string& file, const float volume);
	virtual void RequestPlaySound(const std::string& file, const float volume);
private:
	void SoundThreadLoop(std::stop_token stopToken);

	enum class SoundRequestType
	{
		PlaySound,
		PlayMusic,
		StopSound,
		StopMusic
	};

	struct SoundData
	{
		SoundRequestType type;
		std::string file;
		float volume;
	};
	std::jthread m_SoundThread;
	std::queue<SoundData> m_SoundQueue;
	std::mutex m_QueueMutex;
	std::condition_variable m_Condition;

	void PlayMusic(const SoundData& soundData);
	void PlaySound(const SoundData& soundData);
	Mix_Music* LoadMusic(const std::string& file);
	Mix_Chunk* LoadChunk(const std::string& file);

	// Define A Custom Deleter So We Don't Have To Worry About Having To Work With Raw Pointers
	struct MixMusicDeleter
	{
		void operator()(Mix_Music* music) const
		{
			Mix_FreeMusic(music);
		}
	};
	std::unordered_map<std::string, std::unique_ptr<Mix_Music, MixMusicDeleter>> m_LoadedMusic;

	// Define A Custom Deleter So We Don't Have To Worry About Having To Work With Raw Pointers
	struct MixChunkDeleter
	{
		void operator()(Mix_Chunk* chunk) const
		{
			Mix_FreeChunk(chunk);
		}
	};
	std::unordered_map<std::string, std::unique_ptr<Mix_Chunk, MixChunkDeleter>> m_LoadedChunks;
};


Twengine::SDLSoundSystem::SDLSoundSystem()
	: m_pImplSDLSoundSystem{new SDLSoundSystemImpl()}
{
}

Twengine::SDLSoundSystem::~SDLSoundSystem()
{
	delete m_pImplSDLSoundSystem;
}

void Twengine::SDLSoundSystem::RequestPlayMusic(const std::string& file, const float volume)
{
	m_pImplSDLSoundSystem->RequestPlayMusic(file, volume);
}

void Twengine::SDLSoundSystem::RequestPlaySound(const std::string& file, const float volume)
{
	m_pImplSDLSoundSystem->RequestPlaySound(file, volume);
}

Twengine::SDLSoundSystem::SDLSoundSystemImpl::SDLSoundSystemImpl()
{
	SDL_Init(SDL_INIT_AUDIO);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	m_SoundThread = std::jthread([this](std::stop_token stopToken)
		{
			SoundThreadLoop(stopToken);
		});
}

Twengine::SDLSoundSystem::SDLSoundSystemImpl::~SDLSoundSystemImpl()
{
	// Make Sure The Thread Isn't Stuck In Waiting Infinitely
	m_Condition.notify_one();
	m_SoundThread.request_stop();

	Mix_CloseAudio();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

void Twengine::SDLSoundSystem::SDLSoundSystemImpl::RequestPlayMusic(const std::string& file, const float volume)
{
	std::lock_guard<std::mutex> lock(m_QueueMutex);
	m_SoundQueue.push({ SoundRequestType::PlayMusic, file, volume });
	m_Condition.notify_one();
}

void Twengine::SDLSoundSystem::SDLSoundSystemImpl::RequestPlaySound(const std::string& file, const float volume)
{
	std::lock_guard<std::mutex> lock(m_QueueMutex);
	m_SoundQueue.push({ SoundRequestType::PlaySound, file, volume });
	m_Condition.notify_one();
}

void Twengine::SDLSoundSystem::SDLSoundSystemImpl::SoundThreadLoop(std::stop_token stopToken)
{
	while (!stopToken.stop_requested())
	{
		std::unique_lock<std::mutex> lock(m_QueueMutex);

		// Wait Until Something Gets Added To Queue Or We Request For The Thread To Stop
		m_Condition.wait(lock, [this, &stopToken] { return !m_SoundQueue.empty() || stopToken.stop_requested(); });

		if (stopToken.stop_requested()) break;

		SoundData soundData = m_SoundQueue.front();
		m_SoundQueue.pop();

		switch (soundData.type)
		{
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

void Twengine::SDLSoundSystem::SDLSoundSystemImpl::PlayMusic(const SoundData& soundData)
{
	Mix_Music* music = LoadMusic(soundData.file);
	Mix_VolumeMusic(static_cast<int>(soundData.volume * MIX_MAX_VOLUME));
	Mix_PlayMusic(music, -1);
}

void Twengine::SDLSoundSystem::SDLSoundSystemImpl::PlaySound(const SoundData& soundData)
{
	Mix_Chunk* chunk = LoadChunk(soundData.file);
	Mix_VolumeChunk(chunk, static_cast<int>(soundData.volume * MIX_MAX_VOLUME));
	int channel = Mix_PlayChannel(-1, chunk, 0);
	Mix_SetPosition(channel, 90, 50);
}

Mix_Music* Twengine::SDLSoundSystem::SDLSoundSystemImpl::LoadMusic(const std::string& file)
{
	if (m_LoadedMusic.find(file) == m_LoadedMusic.end())
		m_LoadedMusic.insert(std::pair(file, Mix_LoadMUS(file.c_str())));
	return m_LoadedMusic.at(file).get();
}

Mix_Chunk* Twengine::SDLSoundSystem::SDLSoundSystemImpl::LoadChunk(const std::string& file)
{
	if (m_LoadedChunks.find(file) == m_LoadedChunks.end())
		m_LoadedChunks.insert(std::pair(file, Mix_LoadWAV(file.c_str())));
	return m_LoadedChunks.at(file).get();
}
