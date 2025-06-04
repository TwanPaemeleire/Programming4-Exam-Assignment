#pragma once
#include <string>
#include <memory>
#include "Component.h"
#include <SDL_ttf.h>

namespace Twengine
{
	class Font;
	class Texture2D;

	class TextComponent final : public Component
	{
	public:
		TextComponent(GameObject* owner);
		virtual ~TextComponent() = default;
		TextComponent(const TextComponent& other) = delete;
		TextComponent(TextComponent&& other) = delete;
		TextComponent& operator=(const TextComponent& other) = delete;
		TextComponent& operator=(TextComponent&& other) = delete;

		virtual void Update() override;
		virtual void Render() const override;

		void SetText(const std::string& text);
		void SetFont(Font* font);
		void SetColor(int r, int g, int b, int a = 255);
		void SetColor(SDL_Color color);

	private:
		bool m_NeedsUpdate;
		std::string m_Text;
		Font* m_Font;
		std::unique_ptr<Texture2D> m_TextTexture;
		SDL_Color m_TextColor;
	};
}