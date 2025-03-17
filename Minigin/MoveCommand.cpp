#include "MoveCommand.h"
#include <iostream>
#include "GameObject.h"
#include "Time.h"

MoveCommand::MoveCommand(Twengine::GameObject* gameObject)
	:GameObjectCommand(gameObject)
{
}

void MoveCommand::Execute()
{
	glm::vec3 movement = glm::vec3(m_Direction * Twengine::Time::GetInstance().deltaTime, 0);
	GetGameObject()->GetTransform()->SetLocalPosition(GetGameObject()->GetTransform()->GetLocalPosition() + movement);
}
