#pragma once
#include <SDL.h>
#include "Singleton.h"

namespace Twengine
{
	class Texture2D;

	class Renderer final : public Singleton<Renderer>
	{
	public:
		void Init(SDL_Window* window);
		void Render() const;
		void Destroy();

		void DrawPoint(float x, float y, const SDL_Color& color, int size) const;
		void DrawRectangle(float x, float y, float width, float height, const SDL_Color& color) const;
		void RenderTexture(const Texture2D& texture, float x, float y, float scaling = 1.f) const;
		void RenderTexture(const Texture2D& texture, float x, float y, float width, float height) const;
		void RenderTextureRect(const Texture2D& texture, float x, float y, float srcX, float srcY, float srcWidth, float srcHeight, bool flipHorizontal = false, bool flipVertical = false, double angle = 0);

		SDL_Renderer* GetSDLRenderer() const;

		const SDL_Color& GetBackgroundColor() const { return m_ClearColor; }
		void SetBackgroundColor(const SDL_Color& color) { m_ClearColor = color; }

		int GetWindowWidth() const { return m_WindowWidth; }
		int GetWindowHeight() const { return m_WindowHeight; }
	private:
		SDL_Renderer* m_Renderer{};
		SDL_Window* m_Window{};
		SDL_Color m_ClearColor{};

		int m_WindowWidth{};
		int m_WindowHeight{};
	};
}


