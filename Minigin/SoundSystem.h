#pragma once
#include <string>
#include "SdbmHash.h"
using SoundId = unsigned int;

namespace Twengine
{
	class SoundSystem
	{
	public:
		virtual ~SoundSystem() = default;
		virtual void RequestLoadMusic(const std::string& file, SoundId id) = 0;
		virtual void RequestLoadSound(const std::string& file, SoundId id) = 0;
		virtual void RequestPlayMusic(SoundId id, const float volume) = 0;
		virtual void RequestPlaySound(SoundId id, const float volume) = 0;
		virtual void RequestMuteAllSound() = 0;
		virtual void RequestUnMuteAllSound() = 0;
		virtual void RequestStopAllSound() =0;
		virtual bool IsMuted() const = 0;
	};

	class NullSoundSystem final : public SoundSystem
	{
	public:
		virtual void RequestLoadMusic(const std::string&, SoundId) override {};
		virtual void RequestLoadSound(const std::string&, SoundId ) override {};
		virtual void RequestPlayMusic(SoundId, const float) override {};
		virtual void RequestPlaySound(SoundId, const float) override {};
		virtual void RequestMuteAllSound() override {};
		virtual void RequestUnMuteAllSound() override {};
		virtual void RequestStopAllSound() override{};
		virtual bool IsMuted() const override{ return false; }
	};

}