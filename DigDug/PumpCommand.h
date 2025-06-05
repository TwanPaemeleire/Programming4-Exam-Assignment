#pragma once
#include "GameObjectCommand.h"
#include "InputManager.h"
class DigDugComponent;

class PumpCommand final : public Twengine::GameObjectCommand
{
public:
	PumpCommand(Twengine::GameObject* gameObject);
	virtual void Execute() override;
	void IsPressBound() { m_InteractionState = Twengine::InteractionStates::pressed; }
	void IsReleaseBound() { m_InteractionState = Twengine::InteractionStates::up; }
	void IsDownBound() { m_InteractionState = Twengine::InteractionStates::down; }

private:
	Twengine::InteractionStates m_InteractionState{};
	DigDugComponent* m_DigDugComponent{};
};

