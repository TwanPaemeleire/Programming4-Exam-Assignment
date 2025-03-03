#include "MoveCommands.h"
#include <iostream>
#include "GameObject.h"
#include "Time.h"

// UP
MoveUpCommand::MoveUpCommand(GameObject* gameObject)
	:GameObjectCommand(gameObject)
{
}

void MoveUpCommand::Execute()
{
	glm::vec3 movement = glm::vec3(0, -m_Speed * Time::GetInstance().deltaTime, 0);
	GetGameObject()->GetTransform()->SetLocalPosition(GetGameObject()->GetTransform()->GetLocalPosition() + movement);
}

// DOWN
MoveDownCommand::MoveDownCommand(GameObject* gameObject)
	:GameObjectCommand(gameObject)
{
}

void MoveDownCommand::Execute()
{
	glm::vec3 movement = glm::vec3(0, m_Speed * Time::GetInstance().deltaTime, 0);
	GetGameObject()->GetTransform()->SetLocalPosition(GetGameObject()->GetTransform()->GetLocalPosition() + movement);
}

// LEFT
MoveLeftCommand::MoveLeftCommand(GameObject* gameObject)
	:GameObjectCommand(gameObject)
{
}

void MoveLeftCommand::Execute()
{
	glm::vec3 movement = glm::vec3(-m_Speed * Time::GetInstance().deltaTime, 0, 0);
	GetGameObject()->GetTransform()->SetLocalPosition(GetGameObject()->GetTransform()->GetLocalPosition() + movement);
}

// RIGHT
MoveRightCommand::MoveRightCommand(GameObject* gameObject)
	:GameObjectCommand(gameObject)
{
}

void MoveRightCommand::Execute()
{
	glm::vec3 movement = glm::vec3(m_Speed * Time::GetInstance().deltaTime, 0, 0);
	GetGameObject()->GetTransform()->SetLocalPosition(GetGameObject()->GetTransform()->GetLocalPosition() + movement);
}
