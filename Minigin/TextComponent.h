#pragma once
#include <string>
#include <memory>
#include "Component.h"

class Font;
class Texture2D;
class TextComponent final : public Component
{
public:
	virtual void Update() override;
	virtual void Render() const override;

	void SetText(const std::string& text);

	TextComponent(const std::string& text, Font* font);
	virtual ~TextComponent() = default;
	TextComponent(const TextComponent& other) = delete;
	TextComponent(TextComponent&& other) = delete;
	TextComponent& operator=(const TextComponent& other) = delete;
	TextComponent& operator=(TextComponent&& other) = delete;
private:
	bool m_needsUpdate;
	std::string m_text;
	Font* m_font;
	std::unique_ptr<Texture2D> m_textTexture;
};
