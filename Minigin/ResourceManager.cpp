#include <stdexcept>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "ResourceManager.h"
//#include "Renderer.h"

namespace fs = std::filesystem;

void Twengine::ResourceManager::Init()
{
	if (TTF_Init() != 0)
	{
		throw std::runtime_error(std::string("Failed to load support for fonts: ") + SDL_GetError());
	}
}

Twengine::Texture2D* Twengine::ResourceManager::LoadTexture(const std::string& file)
{
	if(m_LoadedTextures.find(file) == m_LoadedTextures.end())
		m_LoadedTextures.insert(std::pair(file,std::make_unique<Texture2D>(file)));
	return m_LoadedTextures.at(file).get();
}

Twengine::Font* Twengine::ResourceManager::LoadFont(const std::string& file, uint8_t size)
{
	const auto key = std::pair<std::string, uint8_t>(file, size);
	if(m_LoadedFonts.find(key) == m_LoadedFonts.end())
		m_LoadedFonts.insert(std::pair(key,std::make_unique<Font>(file, size)));
	return m_LoadedFonts.at(key).get();
}

Mix_Music* Twengine::ResourceManager::LoadMusic(const std::string& file)
{
	if (m_LoadedMusic.find(file) == m_LoadedMusic.end())
		m_LoadedMusic.insert(std::pair(file, Mix_LoadMUS(file.c_str())));
	return m_LoadedMusic.at(file).get();
}

Mix_Chunk* Twengine::ResourceManager::LoadChunk(const std::string& file)
{
	if (m_LoadedChunks.find(file) == m_LoadedChunks.end())
		m_LoadedChunks.insert(std::pair(file, Mix_LoadWAV(file.c_str())));
	return m_LoadedChunks.at(file).get();
}
