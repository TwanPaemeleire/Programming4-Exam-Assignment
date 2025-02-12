#include <stdexcept>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include "ResourceManager.h"
#include "Renderer.h"

namespace fs = std::filesystem;

void ResourceManager::Init(const std::filesystem::path& dataPath)
{
	m_dataPath = dataPath;

	if (TTF_Init() != 0)
	{
		throw std::runtime_error(std::string("Failed to load support for fonts: ") + SDL_GetError());
	}
}

Texture2D* ResourceManager::LoadTexture(const std::string& file)
{
	const auto fullPath = m_dataPath/file;
	const auto filename = fs::path(fullPath).filename().string();
	if(m_LoadedTextures.find(filename) == m_LoadedTextures.end())
		m_LoadedTextures.insert(std::pair(filename,std::make_unique<Texture2D>(fullPath.string())));
	return m_LoadedTextures.at(filename).get();
}

Font* ResourceManager::LoadFont(const std::string& file, uint8_t size)
{
	const auto fullPath = m_dataPath/file;
	const auto filename = fs::path(fullPath).filename().string();
	const auto key = std::pair<std::string, uint8_t>(filename, size);
	if(m_LoadedFonts.find(key) == m_LoadedFonts.end())
		m_LoadedFonts.insert(std::pair(key,std::make_unique<Font>(fullPath.string(), size)));
	return m_LoadedFonts.at(key).get();
}

//void ResourceManager::UnloadUnusedResources()
//{
//	for (auto it = m_loadedTextures.begin(); it != m_loadedTextures.end();)
//	{
//		if (it->second.use_count() == 1)
//			it = m_loadedTextures.erase(it);
//		else
//			++it;
//	}
//
//	for (auto it = m_loadedFonts.begin(); it != m_loadedFonts.end();)
//	{
//		if (it->second.use_count() == 1)
//			it = m_loadedFonts.erase(it);
//		else
//			++it;
//	}
//}
