#include "JoystickMoveCommand.h"
#include "GameObject.h"
#include "DigDugComponent.h"

JoystickMoveCommand::JoystickMoveCommand(Twengine::GameObject* gameObject)
	:Twengine::JoystickCommand(gameObject)
{
	m_DigDugComp = gameObject->GetComponent<DigDugComponent>();
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
	m_DigDugComp->SetDirection(glm::vec2(x, -y));
}
