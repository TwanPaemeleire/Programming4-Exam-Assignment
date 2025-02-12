#pragma once
#include "Component.h"
#include <memory>
#include <string>

class Texture2D;

class TextureRenderComponent final : public Component
{
public:
	TextureRenderComponent(const std::string& imgPath);
	virtual ~TextureRenderComponent() override = default;
	TextureRenderComponent(const TextureRenderComponent& other) = delete;
	TextureRenderComponent(TextureRenderComponent&& other) = delete;
	TextureRenderComponent& operator=(const TextureRenderComponent& other) = delete;
	TextureRenderComponent& operator=(TextureRenderComponent&& other) = delete;

	virtual void Render() const override;

private:
	Texture2D* m_Texture;
};

