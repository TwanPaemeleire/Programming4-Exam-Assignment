#pragma once
#include "Component.h"
#include <memory>
#include <string>

class Texture2D;

class TextureRenderComponent : public Component
{
public:
	virtual void Render() const override;

	TextureRenderComponent(const std::string& imgPath);
	virtual ~TextureRenderComponent() override = default;

	TextureRenderComponent(const TextureRenderComponent& other) = delete;
	TextureRenderComponent(TextureRenderComponent&& other) = delete;
	TextureRenderComponent& operator=(const TextureRenderComponent& other) = delete;
	TextureRenderComponent& operator=(TextureRenderComponent&& other) = delete;
private:
	Texture2D* m_Texture;
};

