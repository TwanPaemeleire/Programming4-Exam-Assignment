#include "TransformComponent.h"
#include "GameObject.h"

TransformComponent::TransformComponent(GameObject* owner)
	:Component(owner)
{
}

void TransformComponent::SetLocalPosition(const glm::vec3& localPos)
{
	m_LocalPosition = localPos;
	SetPositionDirty();
}

void TransformComponent::SetLocalPosition(float x, float y)
{
	SetLocalPosition(glm::vec3(x, y, 0));
}

const glm::vec3& TransformComponent::GetWorldPosition()
{
	if (m_PositionIsDirty)
	{
		UpdateWorldPosition();
	}
	return m_WorldPosition;
}

void TransformComponent::SetPositionDirty()
{
	m_PositionIsDirty = true;
	const size_t amountOfChildren = GetOwner()->GetChildCount();
	for (size_t childIdx{0}; childIdx < amountOfChildren; ++childIdx)
	{
		GetOwner()->GetChildAt(int(childIdx))->GetTransform()->SetPositionDirty();
	}
}

void TransformComponent::UpdateWorldPosition()
{
	if (m_PositionIsDirty)
	{
		if (GetOwner()->GetParent() == nullptr)
		{
			m_WorldPosition = m_LocalPosition;
		}
		else
		{
			m_WorldPosition = GetOwner()->GetParent()->GetTransform()->GetWorldPosition() + m_LocalPosition;
		}
	}
	m_PositionIsDirty = false;
}
