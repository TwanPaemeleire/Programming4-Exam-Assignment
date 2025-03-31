#pragma once
#include <memory>
#include "SoundSystem.h"

namespace Twengine
{
	class ServiceLocator final
	{
	public:
		static SoundSystem& get_sound_system() { return *s_SoundSystem; }
		static void register_sound_system(std::unique_ptr<SoundSystem>&& soundSystem)
		{
			s_SoundSystem = std::move(soundSystem);
		}
	private:
		static std::unique_ptr<SoundSystem> s_SoundSystem;
	};
}