#include "MoveCommand.h"
#include <iostream>

MoveCommand::MoveCommand(GameObject* gameObject, int speed)
	:GameObjectCommand(gameObject), m_Speed{speed}
{
}

void MoveCommand::Execute()
{
	std::cout << "MOVE" << std::endl;
}
