#pragma once
#include "JoystickCommand.h"

class DigDugMovementComponent;

class JoystickMoveCommand final : public Twengine::JoystickCommand
{
public:
	JoystickMoveCommand(Twengine::GameObject* gameObject);
	virtual void Execute(float x, float y) override;

private:
	DigDugMovementComponent* m_MovementComp;
	float m_Speed{ 20.f };
};

