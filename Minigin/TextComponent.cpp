#include <stdexcept>
#include "TextComponent.h"
#include "Renderer.h"
#include "Font.h"
#include "Texture2D.h"
#include <algorithm>
#include "TransformComponent.h"
#include "GameObject.h"

Twengine::TextComponent::TextComponent(Twengine::GameObject* owner)
	: Component(owner),
	m_NeedsUpdate(true), m_Text{"NO TEXT"}, m_Font{}, m_TextTexture(nullptr), m_TextColor{255, 255, 255, 255}
{
}

void Twengine::TextComponent::Update()
{
	if (m_NeedsUpdate)
	{
		const auto surf = TTF_RenderText_Blended(m_Font->GetFont(), m_Text.c_str(), m_TextColor);
		if (surf == nullptr) 
		{
			throw std::runtime_error(std::string("Render text failed: ") + SDL_GetError());
		}
		auto texture = SDL_CreateTextureFromSurface(Twengine::Renderer::GetInstance().GetSDLRenderer(), surf);
		if (texture == nullptr) 
		{
			throw std::runtime_error(std::string("Create text texture from surface failed: ") + SDL_GetError());
		}
		SDL_FreeSurface(surf);
		m_TextTexture = std::make_unique<Twengine::Texture2D>(texture);
		m_NeedsUpdate = false;
	}
}

void Twengine::TextComponent::Render() const
{
	if (m_TextTexture != nullptr)
	{
		const auto& pos = m_Transform->GetWorldPosition();
		Twengine::Renderer::GetInstance().RenderTexture(*m_TextTexture, pos.x, pos.y);
	}
}

void Twengine::TextComponent::SetText(const std::string& text)
{
	m_Text = text;
	m_NeedsUpdate = true;
}

void Twengine::TextComponent::SetFont(Twengine::Font* font)
{
	m_Font = font;
}

void Twengine::TextComponent::SetColor(int r, int g, int b, int a)
{
	Uint8 red = Uint8(std::clamp(r, 0, 255));
	Uint8 green = Uint8(std::clamp(g, 0, 255));
	Uint8 blue = Uint8(std::clamp(b, 0, 255));
	Uint8 alpha = Uint8(std::clamp(a, 0, 255));
	m_TextColor = SDL_Color(red, green, blue, alpha);
	m_NeedsUpdate = true;
}


