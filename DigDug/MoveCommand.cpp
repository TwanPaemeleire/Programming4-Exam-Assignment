#include "MoveCommand.h"
#include <iostream>
#include "GameObject.h"
#include "MyTime.h"
#include "AnimationComponent.h"
#include "DigDugMovementComponent.h"

MoveCommand::MoveCommand(Twengine::GameObject* gameObject)
	:GameObjectCommand(gameObject)
{
	m_AnimationComponent = gameObject->GetComponent<Twengine::AnimationComponent>();
	m_MovementComp = gameObject->GetComponent<DigDugMovementComponent>();
}

void MoveCommand::Execute()
{
	glm::vec3 movement = glm::vec3(m_Direction * Twengine::Time::GetInstance().deltaTime, 0);
	//GetGameObject()->GetTransform()->SetLocalPosition(GetGameObject()->GetTransform()->GetLocalPosition() + movement);
	m_MovementComp->SetXDirection(m_Direction.x);
	m_MovementComp->SetYDirection(m_Direction.y);

	if (m_AnimationComponent == nullptr) return;
	if (movement.x > 0) // Moving Right
	{
		m_AnimationComponent->SetRotationAngle(0);
		m_AnimationComponent->SetFlipHorizontal(false);
	}
	else if (movement.x < 0) // Moving Left
	{
		m_AnimationComponent->SetRotationAngle(0);
		m_AnimationComponent->SetFlipHorizontal(true);
	}
	else if (movement.y > 0) // Moving Up
	{
		m_AnimationComponent->SetRotationAngle(90);
		m_AnimationComponent->SetFlipHorizontal(false);
	}
	else if (movement.y < 0) // Moving Down
	{
		m_AnimationComponent->SetRotationAngle(90);
		m_AnimationComponent->SetFlipHorizontal(true);
	}
}
