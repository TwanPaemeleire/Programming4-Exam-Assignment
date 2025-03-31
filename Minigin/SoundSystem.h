#pragma once
#include <string>

namespace Twengine
{
	class SoundSystem
	{
	public:
		virtual ~SoundSystem() = default;
		virtual void PlayMusic(const std::string& file, const float volume) = 0;
		virtual void PlaySound(const std::string& file, const float volume) = 0;
	};
}