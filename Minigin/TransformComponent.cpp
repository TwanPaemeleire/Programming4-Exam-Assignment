#include "TransformComponent.h"

TransformComponent::TransformComponent(GameObject* owner, TransformComponent* transform)
	:Component(owner, transform), m_Position{}
{
}

void TransformComponent::SetPosition(const float x, const float y, const float z)
{
	m_Position.x = x;
	m_Position.y = y;
	m_Position.z = z;
}
