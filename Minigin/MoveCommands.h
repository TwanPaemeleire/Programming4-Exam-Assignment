#pragma once
#include "GameObjectCommand.h"

class MoveUpCommand : public GameObjectCommand
{
public:
	MoveUpCommand(GameObject* gameObject);
	virtual void Execute() override;

private:
	int m_Speed{};
};

