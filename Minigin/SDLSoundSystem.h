#pragma once
#include "SoundSystem.h"

namespace Twengine
{
	class SDLSoundSystem final : public SoundSystem
	{
	public:
		virtual void Play(const SoundId id, const float volume) override;

	private:
	};
}