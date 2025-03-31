#include "SDLSoundSystem.h"
#include <iostream>
#include "SDL.h"

Twengine::SDLSoundSystem::SDLSoundSystem()
{
	SDL_Init(SDL_INIT_AUDIO);
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
}

void Twengine::SDLSoundSystem::Play(const SoundId id, const float volume)
{
	std::unique_ptr<Mix_Music, MixMusicDeleter> music(Mix_LoadMUS("Level/LevelMusic.wav"));
	Mix_VolumeMusic(static_cast<int>(volume * MIX_MAX_VOLUME));
	Mix_PlayMusic(music.get(), -1);
	m_MusicMixs[id] = std::move(music);
}