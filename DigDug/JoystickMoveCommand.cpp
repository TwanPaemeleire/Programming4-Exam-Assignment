#include "JoystickMoveCommand.h"
#include <iostream>
#include "glm.hpp"
#include "GameObject.h"
#include "AnimationComponent.h"
#include "MyTime.h"
#include "DigDugMovementComponent.h"	

JoystickMoveCommand::JoystickMoveCommand(Twengine::GameObject* gameObject)
	:Twengine::JoystickCommand(gameObject)
{
	m_AnimationComponent = gameObject->GetComponent<Twengine::AnimationComponent>();
	m_MovementComp = gameObject->GetComponent<DigDugMovementComponent>();
}

void JoystickMoveCommand::Execute(float x, float y)
{
	if (!(x == 0.f && y == 0.f))
	{
		if (abs(x) > abs(y))
		{
			y = 0.f;
			x = (x > 0.f) ? 1.f : -1.f;
		}
		else
		{
			x = 0.f;
			y = (y > 0.f) ? 1.f : -1.f;
		}
	}
	glm::vec3 movement = glm::vec3(x * m_Speed * Twengine::Time::GetInstance().deltaTime, -y * m_Speed * Twengine::Time::GetInstance().deltaTime, 0);
	//GetGameObject()->GetTransform()->SetLocalPosition(GetGameObject()->GetTransform()->GetLocalPosition() + movement);
	m_MovementComp->SetXDirection(x);
	m_MovementComp->SetYDirection(-y);

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
