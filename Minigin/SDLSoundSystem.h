#pragma once
#include "SoundSystem.h"
#include <unordered_map>
#include <memory>

namespace Twengine
{
	class SDLSoundSystem final : public SoundSystem
	{
	public:
		SDLSoundSystem();
		virtual void PlayMusic(const std::string& file, const float volume) override;
		virtual void PlaySound(const std::string& file, const float volume) override;
	};
}