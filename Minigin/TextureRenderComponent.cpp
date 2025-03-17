#include "TextureRenderComponent.h"
#include "Texture2D.h"
#include "Renderer.h"
#include "GameObject.h"
#include "ResourceManager.h"

Twengine::TextureRenderComponent::TextureRenderComponent(GameObject* owner)
	: Component(owner), m_Texture{}
{
}

void Twengine::TextureRenderComponent::Render() const
{
	auto& pos = m_Transform->GetWorldPosition();
	Renderer::GetInstance().RenderTexture(*m_Texture, pos.x, pos.y);
}

void Twengine::TextureRenderComponent::SetTexture(const std::string& imgPath)
{
	m_Texture = ResourceManager::GetInstance().LoadTexture(imgPath);
}
