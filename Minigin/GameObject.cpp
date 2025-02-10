#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

#include "Time.h"
#include "Component.h"

GameObject::GameObject()
{
	std::unique_ptr<Transform> transform = std::make_unique<Transform>();
	m_Transform = transform.get();
	AddComponent(std::move(transform));
}

void GameObject::Start()
{
	for (auto& component : m_Components)
	{
		component->Start();
	}
}

void GameObject::Update()
{
	for (auto& component : m_Components)
	{
		component->Update();
	}
}

void GameObject::FixedUpdate()
{
	for (auto& component : m_Components)
	{
		component->FixedUpdate();
	}
}

void GameObject::Render() const
{
	//const auto& pos = m_Transform->GetPosition();
	//Renderer::GetInstance().RenderTexture(*m_texture, pos.x, pos.y);
	for (auto& component : m_Components)
	{
		component->Render();
	}
}

void GameObject::AddComponent(std::unique_ptr<Component> component)
{
	component->m_ParentGameObject = this;
	component->m_Transform = m_Transform;
	m_Components.emplace_back(std::move(component));
}

//void GameObject::SetTexture(const std::string& filename)
//{
//	m_texture = ResourceManager::GetInstance().LoadTexture(filename);
//}

void GameObject::SetPosition(float x, float y)
{
	m_Transform->SetPosition(x, y, 0.0f);
}