#pragma once
#include "SoundSystem.h"
#include "SDL_mixer.h"
#include <unordered_map>
#include <memory>

namespace Twengine
{
	class SDLSoundSystem final : public SoundSystem
	{
	public:
		SDLSoundSystem();
		virtual void Play(const SoundId id, const float volume) override;
	private:
		struct MixMusicDeleter
		{
			void operator()(Mix_Music* music) const
			{
				Mix_FreeMusic(music);
			}
		};

		std::unordered_map<SoundId, std::unique_ptr<Mix_Music, MixMusicDeleter>> m_MusicMixs;
	};
}