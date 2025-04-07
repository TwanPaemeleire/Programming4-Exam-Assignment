#pragma once
#include "SoundSystem.h"
#include <unordered_map>
#include <memory>
#include "SDL_mixer.h"
#include <queue>
#include <condition_variable>
#include <thread>

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
		void SoundThreadLoop(std::stop_token stopToken);

		enum class SoundRequestType
		{
			PlaySound,
			PlayMusic,
			StopSound,
			StopMusic
		};

		struct SoundData
		{
			SoundRequestType type;
			std::string file;
			float volume;
		};
		std::jthread m_SoundThread;
		std::queue<SoundData> m_SoundQueue;
		std::mutex m_QueueMutex;
		std::condition_variable m_Condition;

		void PlayMusic(const SoundData& soundData);
		void PlaySound(const SoundData& soundData);
		Mix_Music* LoadMusic(const std::string& file);
		Mix_Chunk* LoadChunk(const std::string& file);

		// Define A Custom Deleter So We Don't Have To Worry About Having To Work With Raw Pointers
		struct MixMusicDeleter
		{
			void operator()(Mix_Music* music) const
			{
				Mix_FreeMusic(music);
			}
		};
		std::unordered_map<std::string, std::unique_ptr<Mix_Music, MixMusicDeleter>> m_LoadedMusic;

		// Define A Custom Deleter So We Don't Have To Worry About Having To Work With Raw Pointers
		struct MixChunkDeleter
		{
			void operator()(Mix_Chunk* chunk) const
			{
				Mix_FreeChunk(chunk);
			}
		};
		std::unordered_map<std::string, std::unique_ptr<Mix_Chunk, MixChunkDeleter>> m_LoadedChunks;
	};
}