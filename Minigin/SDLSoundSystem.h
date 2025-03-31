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
		virtual void Play(const std::string& file, const float volume) override;
	};
}