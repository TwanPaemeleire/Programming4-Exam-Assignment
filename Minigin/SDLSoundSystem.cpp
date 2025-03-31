#include "SDLSoundSystem.h"
#include <iostream>
#include "SDL.h"
#include "ResourceManager.h"

Twengine::SDLSoundSystem::SDLSoundSystem()
{
	SDL_Init(SDL_INIT_AUDIO);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
}

void Twengine::SDLSoundSystem::Play(const std::string& file, const float volume)
{
	Mix_Music* music = ResourceManager::GetInstance().LoadMusic(file);
	Mix_VolumeMusic(static_cast<int>(volume * MIX_MAX_VOLUME));
	Mix_PlayMusic(music, -1);
}