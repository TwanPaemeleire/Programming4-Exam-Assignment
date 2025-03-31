#pragma once
#include <string>

namespace Twengine
{
	class SoundSystem
	{
	public:
		virtual ~SoundSystem() = default;
		virtual void Play(const std::string& file, const float volume) = 0;
	};
}