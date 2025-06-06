#pragma once
#include <vec2.hpp>
#include <string>

struct SDL_Texture;
namespace Twengine
{
	class Texture2D final
	{
	public:
		Texture2D(const Texture2D&) = delete;
		Texture2D(Texture2D&&) = delete;
		Texture2D& operator= (const Texture2D&) = delete;
		Texture2D& operator= (const Texture2D&&) = delete;

		SDL_Texture* GetSDLTexture() const;
		explicit Texture2D(SDL_Texture* texture);
		explicit Texture2D(const std::string& fullPath);
		~Texture2D();

		glm::ivec2 GetSize() const;

	private:
		SDL_Texture* m_Texture;
	};
}