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
	m_MovementComp->SetXDirection(x);
	m_MovementComp->SetYDirection(-y);
}
