#include "MoveCommand.h"
#include <iostream>
#include "GameObject.h"
#include "MyTime.h"
#include "AnimationComponent.h"
#include "DigDugMovementComponent.h"

MoveCommand::MoveCommand(Twengine::GameObject* gameObject)
	:GameObjectCommand(gameObject)
{
	m_MovementComp = gameObject->GetComponent<DigDugMovementComponent>();
}

void MoveCommand::Execute()
{
	m_MovementComp->SetXDirection(m_Direction.x);
	m_MovementComp->SetYDirection(m_Direction.y);
}
