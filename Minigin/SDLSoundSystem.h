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
		virtual void RequestLoadMusic(const std::string& file, SoundId id) override;
		virtual void RequestLoadSound(const std::string& file, SoundId id) override;
		virtual void RequestPlayMusic(SoundId id, const float volume) override;
		virtual void RequestPlaySound(SoundId id, const float volume) override;
		virtual void RequestMuteAllSound() override;
		virtual void RequestUnMuteAllSound() override;
		virtual void RequestStopAllSound() override;
		virtual bool IsMuted() const override;

	private:
		class SDLSoundSystemImpl;
		SDLSoundSystemImpl* m_pImplSDLSoundSystem;
	};
}