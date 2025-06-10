#pragma once
#include "GameObjectCommand.h"

class GoToNextLevelCommand final : public Twengine::GameObjectCommand
{
public:
	GoToNextLevelCommand(Twengine::GameObject* gameObject);
	virtual void Execute() override;
};

