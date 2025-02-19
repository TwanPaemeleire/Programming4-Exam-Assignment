#include "TextureRenderComponent.h"
#include "Texture2D.h"
#include "Renderer.h"
#include "GameObject.h"
#include "ResourceManager.h"

TextureRenderComponent::TextureRenderComponent(GameObject* owner)
	: Component(owner), m_Texture{}
{
}

void TextureRenderComponent::Render() const
{
	auto& pos = m_Transform->GetWorldPosition();
	Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y);
}

void TextureRenderComponent::SetTexture(const std::string& imgPath)
{
	m_Texture = ResourceManager::GetInstance().LoadTexture(imgPath);
}
