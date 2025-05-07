#pragma once
#include "JoystickCommand.h"

class DigDugComponent;

class JoystickMoveCommand final : public Twengine::JoystickCommand
{
public:
	JoystickMoveCommand(Twengine::GameObject* gameObject);
	virtual void Execute(float x, float y) override;

private:
	DigDugComponent* m_DigDugComp;
};

