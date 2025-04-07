#pragma once
#include "SoundSystem.h"


namespace Twengine
{
	class SDLSoundSystem final : public SoundSystem
	{
	public:
		SDLSoundSystem();
		~SDLSoundSystem();
		SDLSoundSystem(const SDLSoundSystem& other) = delete;
		SDLSoundSystem(SDLSoundSystem&& other) = delete;
		SDLSoundSystem& operator=(const SDLSoundSystem& other) = delete;
		SDLSoundSystem& operator=(SDLSoundSystem&& other) = delete;
		virtual void RequestPlayMusic(const std::string& file, const float volume) override;
		virtual void RequestPlaySound(const std::string& file, const float volume) override;

	private:
		class SDLSoundSystemImpl;
		SDLSoundSystemImpl* m_pImplSDLSoundSystem;
	};
}