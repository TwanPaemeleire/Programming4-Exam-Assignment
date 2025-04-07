#pragma once
#include <string>

namespace Twengine
{
	class SoundSystem
	{
	public:
		virtual ~SoundSystem() = default;
		virtual void RequestPlayMusic(const std::string& file, const float volume) = 0;
		virtual void RequestPlaySound(const std::string& file, const float volume) = 0;
	};
}