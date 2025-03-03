#include "MoveCommands.h"
#include <iostream>
#include "GameObject.h"

MoveUpCommand::MoveUpCommand(GameObject* gameObject)
	:GameObjectCommand(gameObject)
{
}

void MoveUpCommand::Execute()
{
	std::cout << "MOVE" << std::endl;
}
