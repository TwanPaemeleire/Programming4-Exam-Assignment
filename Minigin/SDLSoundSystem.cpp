#include "SDLSoundSystem.h"
#include <iostream>
#include "SDL.h"
#include "ResourceManager.h"

Twengine::SDLSoundSystem::SDLSoundSystem()
{
	SDL_Init(SDL_INIT_AUDIO);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
}

void Twengine::SDLSoundSystem::PlayMusic(const std::string& file, const float volume)
{
	Mix_Music* music = LoadMusic(file);
	Mix_VolumeMusic(static_cast<int>(volume * MIX_MAX_VOLUME));
	Mix_PlayMusic(music, -1);
}

void Twengine::SDLSoundSystem::PlaySound(const std::string& file, const float volume)
{
	Mix_Chunk* chunk = LoadChunk(file);
	Mix_VolumeChunk(chunk, static_cast<int>(volume * MIX_MAX_VOLUME));
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
