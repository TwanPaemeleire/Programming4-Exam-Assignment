#pragma once

#include <filesystem>
#include <string>
#include <memory>
#include <map>
#include "Singleton.h"

#include "Texture2D.h"
#include "Font.h"
#include "SDL_mixer.h"

namespace Twengine
{
	class ResourceManager final : public Singleton<ResourceManager>
	{
	public:
		void Init();
		Texture2D* LoadTexture(const std::string& file);
		Font* LoadFont(const std::string& file, uint8_t size);
		Mix_Music* LoadMusic(const std::string& file);
	private:
		friend class Singleton<ResourceManager>;
		ResourceManager() = default;

		std::map<std::string, std::unique_ptr<Texture2D>> m_LoadedTextures;
		std::map<std::pair<std::string, uint8_t>, std::unique_ptr<Font>> m_LoadedFonts;

		// Define A Custom Deleted So We Don't Have To Worry About Having To Work With Raw Pointers
		struct MixMusicDeleter
		{
			void operator()(Mix_Music* music) const
			{
				Mix_FreeMusic(music);
			}
		};
		std::map<std::string, std::unique_ptr<Mix_Music, MixMusicDeleter>> m_LoadedMusic;
	};
}