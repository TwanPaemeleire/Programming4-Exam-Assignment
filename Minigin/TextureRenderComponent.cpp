#include "TextureRenderComponent.h"
#include "Texture2D.h"
#include "Renderer.h"
#include "GameObject.h"
#include "ResourceManager.h"

TextureRenderComponent::TextureRenderComponent(const std::string& imgPath)
{
	m_Texture = ResourceManager::GetInstance().LoadTexture(imgPath).get();
}

void TextureRenderComponent::Render() const
{
	auto& pos = GetTransform()->GetPosition();
	Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y);
}
