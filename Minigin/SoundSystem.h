#pragma once

using SoundId = unsigned int;
namespace Twengine
{
	class SoundSystem
	{
	public:
		virtual ~SoundSystem() = default;
		virtual void Play(const SoundId id, const float volume) = 0;
	};
}