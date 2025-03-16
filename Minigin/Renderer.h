#pragma once
#include <SDL.h>
#include "Singleton.h"

class Texture2D;
/**
 * Simple RAII wrapper for the SDL renderer
 */
class Renderer final : public Singleton<Renderer>
{
public:
	void Init(SDL_Window* window);
	void Render() const;
	void Destroy();

	void RenderTexture(const Texture2D& texture, float x, float y) const;
	void RenderTexture(const Texture2D& texture, float x, float y, float width, float height) const;
	void RenderTextureRect(const Texture2D& texture, float x, float y, float srcX, float srcY, float srcWidth, float srcHeight);

	SDL_Renderer* GetSDLRenderer() const;

	const SDL_Color& GetBackgroundColor() const { return m_ClearColor; }
	void SetBackgroundColor(const SDL_Color& color) { m_ClearColor = color; }
private:
	SDL_Renderer* m_Renderer{};
	SDL_Window* m_Window{};
	SDL_Color m_ClearColor{};
};


