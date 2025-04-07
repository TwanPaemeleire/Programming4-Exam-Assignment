#pragma once
#include "SoundSystem.h"
#include <unordered_map>
#include <memory>
#include "SDL_mixer.h"

namespace Twengine
{
	class SDLSoundSystem final : public SoundSystem
	{
	public:
		SDLSoundSystem();
		virtual void PlayMusic(const std::string& file, const float volume) override;
		virtual void PlaySound(const std::string& file, const float volume) override;

	private:
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
}