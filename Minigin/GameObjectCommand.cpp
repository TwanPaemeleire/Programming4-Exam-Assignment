#include "GameObjectCommand.h"

GameObjectCommand::GameObjectCommand(GameObject* gameObject)
	:m_GameObject{gameObject}
{
}

GameObjectCommand::~GameObjectCommand()
{
}
