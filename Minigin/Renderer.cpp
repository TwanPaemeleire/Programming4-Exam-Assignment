#include <stdexcept>
#include "Renderer.h"
#include "SceneManager.h"
#include "Texture2D.h"
#include "imgui.h"
#include "backends/imgui_impl_sdl2.h"
#include "backends/imgui_impl_opengl3.h"

int GetOpenGLDriverIndex()
{
	auto openglIndex = -1;
	const auto driverCount = SDL_GetNumRenderDrivers();
	for (auto i = 0; i < driverCount; i++)
	{
		SDL_RendererInfo info;
		if (!SDL_GetRenderDriverInfo(i, &info))
			if (!strcmp(info.name, "opengl"))
				openglIndex = i;
	}
	return openglIndex;
}

void Twengine::Renderer::Init(SDL_Window* window)
{
	m_Window = window;
	m_Renderer = SDL_CreateRenderer(window, GetOpenGLDriverIndex(), SDL_RENDERER_ACCELERATED);
	if (m_Renderer == nullptr) 
	{
		throw std::runtime_error(std::string("SDL_CreateRenderer Error: ") + SDL_GetError());
	}

	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui_ImplSDL2_InitForOpenGL(window, SDL_GL_GetCurrentContext());
	ImGui_ImplOpenGL3_Init();
}

void Twengine::Renderer::Render() const
{
	const auto& color = GetBackgroundColor();
	SDL_SetRenderDrawColor(m_Renderer, color.r, color.g, color.b, color.a);
	SDL_RenderClear(m_Renderer);

	SceneManager::GetInstance().Render();

	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplSDL2_NewFrame();
	ImGui::NewFrame();

	SceneManager::GetInstance().RenderUI();

	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	
	SDL_RenderPresent(m_Renderer);
}

void Twengine::Renderer::Destroy()
{
	// TEMP
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplSDL2_Shutdown();
	ImGui::DestroyContext();

	if (m_Renderer != nullptr)
	{
		SDL_DestroyRenderer(m_Renderer);
		m_Renderer = nullptr;
	}
}

void Twengine::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y) const
{
	SDL_Rect dstRect{};
	dstRect.x = static_cast<int>(x);
	dstRect.y = static_cast<int>(y);
	SDL_QueryTexture(texture.GetSDLTexture(), nullptr, nullptr, &dstRect.w, &dstRect.h);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dstRect);
}

void Twengine::Renderer::RenderTexture(const Texture2D& texture, const float x, const float y, const float dstWidth, const float dstHeight) const
{
	SDL_Rect dstRect{};
	dstRect.x = static_cast<int>(x);
	dstRect.y = static_cast<int>(y);
	dstRect.w = static_cast<int>(dstWidth);
	dstRect.h = static_cast<int>(dstHeight);
	SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), nullptr, &dstRect);
}

void Twengine::Renderer::RenderTextureRect(const Texture2D& texture, float x, float y, float srcX, float srcY, float srcWidth, float srcHeight, bool flipHorizontal, bool flipVertical, double angle)
{
	SDL_Rect srcRect{};
	srcRect.x = static_cast<int>(srcX);
	srcRect.y = static_cast<int>(srcY);
	srcRect.w = static_cast<int>(srcWidth);
	srcRect.h = static_cast<int>(srcHeight);
	SDL_Rect dstRect{};
	dstRect.x = static_cast<int>(x);
	dstRect.y = static_cast<int>(y);
	dstRect.w = static_cast<int>(srcWidth);
	dstRect.h = static_cast<int>(srcHeight);

	SDL_RendererFlip flip = SDL_FLIP_NONE;
	if (flipHorizontal) flip = SDL_FLIP_HORIZONTAL;
	if (flipVertical) flip = static_cast<SDL_RendererFlip>(flip | SDL_FLIP_VERTICAL);
	//SDL_RenderCopy(GetSDLRenderer(), texture.GetSDLTexture(), &srcRect, &dstRect);
	SDL_RenderCopyEx(GetSDLRenderer(), texture.GetSDLTexture(), &srcRect, &dstRect, angle, nullptr, flip);
}

SDL_Renderer* Twengine::Renderer::GetSDLRenderer() const { return m_Renderer; }
