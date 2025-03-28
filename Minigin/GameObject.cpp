#include <string>
#include "GameObject.h"
#include "ResourceManager.h"
#include "Renderer.h"

#include "Time.h"
#include "Component.h"

Twengine::GameObject::GameObject()
{
	m_Transform = AddComponent<TransformComponent>();
	m_TagId = make_sdbm_hash("Default");
}

Twengine::GameObject::~GameObject()
{
	// "Notify" Parent That You're Getting Destroyed, And Remove Yourself As Parent From Your Children
	SetParent(nullptr, false);
	for (auto& child : m_Children)
	{
		child->SetParent(nullptr, false);
	}
}

void Twengine::GameObject::Start()
{
	for (auto& component : m_Components)
	{
		component->Start();
	}
}

void Twengine::GameObject::Update()
{
	for (auto& component : m_Components)
	{
		component->Update();
	}
}

void Twengine::GameObject::FixedUpdate()
{
	for (auto& component : m_Components)
	{
		component->FixedUpdate();
	}
}

void Twengine::GameObject::LateUpdate()
{
	for (auto& component : m_Components)
	{
		component->LateUpdate();
	}

	// Delete The Components Marked For Destruction
	std::erase_if(m_Components, [](const auto& component)
		{
			return component->IsMarkedForDestruction();
		});

}

void Twengine::GameObject::Render() const
{
	for (auto& component : m_Components)
	{
		component->Render();
	}
}

void Twengine::GameObject::RenderUI()
{
	for (auto& component : m_Components)
	{
		component->RenderUI();
	}
}

void Twengine::GameObject::MarkForDestruction()
{
	// Mark All The Children (And As Such, Also Their Children) For Destruction As Well, As I Want Parent To Own Children
	m_MarkedForDestruction = true;
	for (auto& child : m_Children)
	{
		child->MarkForDestruction();
	}
}

void Twengine::GameObject::SetParent(GameObject* parent, bool keepWorldPosition)
{
	if (IsChild(parent) || parent == this || m_Parent == parent)
		return;
	if (parent == nullptr)
		m_Transform->SetLocalPosition(m_Transform->GetWorldPosition());
	else
	{
		if (keepWorldPosition)
		{
			m_Transform->SetLocalPosition(m_Transform->GetWorldPosition() - parent->GetTransform()->GetWorldPosition());
		}
		m_Transform->SetPositionDirty();
	}
	if (m_Parent) m_Parent->RemoveChild(this);
	m_Parent = parent;
	if (m_Parent) m_Parent->AddChild(this);
}

void Twengine::GameObject::AddChild(GameObject* child)
{
	m_Children.emplace_back(child);
}

void Twengine::GameObject::RemoveChild(GameObject* child)
{
	m_Children.erase(std::remove(m_Children.begin(), m_Children.end(), child), m_Children.end());
}

bool Twengine::GameObject::IsChild(GameObject* objectToCheck)
{
	for (auto& child : m_Children)
	{
		if (child == objectToCheck) return true;
		if (child->IsChild(objectToCheck)) return true;
	}
	return false;
}
