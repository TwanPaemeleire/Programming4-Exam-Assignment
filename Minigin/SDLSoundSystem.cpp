#include "SDLSoundSystem.h"
#include <iostream>
#include "SDL.h"
#include "ResourceManager.h"
#include <iostream>

Twengine::SDLSoundSystem::SDLSoundSystem()
{
	SDL_Init(SDL_INIT_AUDIO);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048); 
	m_SoundThread = std::jthread([this](std::stop_token stopToken)
		{
			SoundThreadLoop(stopToken);
		});
}

Twengine::SDLSoundSystem::~SDLSoundSystem()
{
	// Make Sure The Thread Isn't Stuck In Waiting Infinitely
	m_Condition.notify_one();
	m_SoundThread.request_stop();

	Mix_CloseAudio();
	SDL_QuitSubSystem(SDL_INIT_AUDIO);
}

void Twengine::SDLSoundSystem::RequestPlayMusic(const std::string& file, const float volume)
{
	std::lock_guard<std::mutex> lock(m_QueueMutex);
	m_SoundQueue.push({ SoundRequestType::PlayMusic, file, volume });
	m_Condition.notify_one();
}

void Twengine::SDLSoundSystem::RequestPlaySound(const std::string& file, const float volume)
{
	std::lock_guard<std::mutex> lock(m_QueueMutex);
	m_SoundQueue.push({ SoundRequestType::PlaySound, file, volume });
	m_Condition.notify_one();
}

void Twengine::SDLSoundSystem::PlayMusic(const SoundData& soundData)
{
	Mix_Music* music = LoadMusic(soundData.file);
	Mix_VolumeMusic(static_cast<int>(soundData.volume * MIX_MAX_VOLUME));
	Mix_PlayMusic(music, -1);
}

void Twengine::SDLSoundSystem::PlaySound(const SoundData& soundData)
{
	Mix_Chunk* chunk = LoadChunk(soundData.file);
	Mix_VolumeChunk(chunk, static_cast<int>(soundData.volume * MIX_MAX_VOLUME));
	int channel = Mix_PlayChannel(-1, chunk, 0);
	Mix_SetPosition(channel, 90, 50);
}

Mix_Music* Twengine::SDLSoundSystem::LoadMusic(const std::string& file)
{
	if (m_LoadedMusic.find(file) == m_LoadedMusic.end())
		m_LoadedMusic.insert(std::pair(file, Mix_LoadMUS(file.c_str())));
	return m_LoadedMusic.at(file).get();
}

Mix_Chunk* Twengine::SDLSoundSystem::LoadChunk(const std::string& file)
{
	if (m_LoadedChunks.find(file) == m_LoadedChunks.end())
		m_LoadedChunks.insert(std::pair(file, Mix_LoadWAV(file.c_str())));
	return m_LoadedChunks.at(file).get();
}

void Twengine::SDLSoundSystem::SoundThreadLoop(std::stop_token stopToken)
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
