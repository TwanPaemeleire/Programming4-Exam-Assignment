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

void GameObject::SetLocalPosition(const glm::vec3& localPos)
{
	m_LocalPosition = localPos;
	SetPositionDirty();
}

const glm::vec3& GameObject::GetWorldPosition()
{
	if (m_PositionIsDirty)
	{
		UpdateWorldPosition();
	}
	return m_WorldPosition;
}

void GameObject::SetParent(GameObject* parent, bool keepWorldPosition)
{
	if (IsChild(parent) || parent == this || m_Parent == parent)
		return;
	if (parent == nullptr)
		SetLocalPosition(GetWorldPosition());
	else
	{
		if (keepWorldPosition)
		{
			SetLocalPosition(GetWorldPosition() - parent->GetWorldPosition());
		}
		SetPositionDirty();
	}
	if (m_Parent) m_Parent->RemoveChild(this);
	m_Parent = parent;
	if (m_Parent) m_Parent->AddChild(this);
}

void GameObject::AddChild(GameObject* child)
{
	m_Children.emplace_back(child);
}

void GameObject::RemoveChild(GameObject* child)
{
	m_Children.erase(std::remove(m_Children.begin(), m_Children.end(), child), m_Children.end());
}

bool GameObject::IsChild(GameObject* objectToCheck)
{
	for (auto& child : m_Children)
	{
		if (child == objectToCheck) return true;
	}

	return false;
}

void GameObject::SetPositionDirty()
{
	m_PositionIsDirty = true;
	for (auto& child : m_Children)
	{
		child->SetPositionDirty();
	}
}

void GameObject::UpdateWorldPosition()
{
	if (m_PositionIsDirty)
	{
		if (m_Parent == nullptr)
		{
			m_WorldPosition = m_LocalPosition;
		}
		else
		{
			m_WorldPosition = m_Parent->GetWorldPosition() + m_LocalPosition;
		}
	}
	m_PositionIsDirty = false;
}
