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
	void Render() const override;

	void SetText(const std::string& text);

	TextComponent(const std::string& text, std::shared_ptr<Font> font);
	virtual ~TextComponent() = default;
	TextComponent(const TextComponent& other) = delete;
	TextComponent(TextComponent&& other) = delete;
	TextComponent& operator=(const TextComponent& other) = delete;
	TextComponent& operator=(TextComponent&& other) = delete;
private:
	bool m_needsUpdate;
	std::string m_text;
	std::shared_ptr<Font> m_font;
	std::shared_ptr<Texture2D> m_textTexture;
};
