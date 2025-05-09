#include "MoveCommand.h"
#include "GameObject.h"
#include "DigDugComponent.h"

MoveCommand::MoveCommand(Twengine::GameObject* gameObject)
	:GameObjectCommand(gameObject)
{
	m_DigDugComp = gameObject->GetComponent<DigDugComponent>();
}

void MoveCommand::Execute()
{
	m_DigDugComp->SetDirection(m_Direction);
}
