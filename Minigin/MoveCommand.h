#pragma once
#include "GameObjectCommand.h"

class MoveCommand : public GameObjectCommand
{
public:
	MoveCommand(GameObject* gameObject, int speed);
	virtual void Execute() override;

private:
	int m_Speed;
};

