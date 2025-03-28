#include "MoveCommand.h"
#include <iostream>
#include "GameObject.h"
#include "MyTime.h"
#include "AnimationComponent.h"

MoveCommand::MoveCommand(Twengine::GameObject* gameObject)
	:GameObjectCommand(gameObject)
{
	m_AnimationComponent = gameObject->GetComponent<Twengine::AnimationComponent>();
}

void MoveCommand::Execute()
{
	glm::vec3 movement = glm::vec3(m_Direction * Twengine::Time::GetInstance().deltaTime, 0);
	GetGameObject()->GetTransform()->SetLocalPosition(GetGameObject()->GetTransform()->GetLocalPosition() + movement);

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
