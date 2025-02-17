#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

#include "Time.h"
#include "Component.h"

GameObject::GameObject()
{
	m_Transform = AddComponent<TransformComponent>();

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

void GameObject::LateUpdate()
{
	// Delete The Components Marked For Destruction
	std::erase_if(m_Components, [](const auto& component)
		{
			return component->IsMarkedForDestruction();
		});

	for (auto& component : m_Components)
	{
		component->LateUpdate();
	}
}

void GameObject::Render() const
{
	for (auto& component : m_Components)
	{
		component->Render();
	}
}

void GameObject::SetPosition(float x, float y)
{
	m_Transform->SetPosition(x, y, 0.0f);
}