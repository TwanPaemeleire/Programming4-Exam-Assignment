#include "DigDugMovementComponent.h"
#include "GameObject.h"
#include "MyTime.h"
#include "GameManager.h"

DigDugMovementComponent::DigDugMovementComponent(Twengine::GameObject* owner)
	:Component(owner)
{
}

void DigDugMovementComponent::Start()
{
	m_GridComponent = GameManager::GetInstance().GetGrid();
}

void DigDugMovementComponent::Update()
{
	if (m_DesiredDirection == glm::vec2(0.f, 0.f)) return;
	auto& oldPos = m_Transform->GetLocalPosition();
	float x = oldPos.x + (m_DesiredDirection.x * m_MovementSpeed * Twengine::Time::GetInstance().deltaTime);
	float y = oldPos.y + (m_DesiredDirection.y * m_MovementSpeed * Twengine::Time::GetInstance().deltaTime);
	m_Transform->SetLocalPosition(x, y);
	m_DesiredDirection.x = 0.f;
	m_DesiredDirection.y = 0.f;
}

void DigDugMovementComponent::SetXDirection(float x)
{
	m_DesiredDirection.x = x;
}

void DigDugMovementComponent::SetYDirection(float y)
{
	m_DesiredDirection.y = y;
}
