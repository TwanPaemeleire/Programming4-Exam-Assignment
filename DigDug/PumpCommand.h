#pragma once
#include "GameObjectCommand.h"

class DigDugComponent;

class PumpCommand final : public Twengine::GameObjectCommand
{
public:
	PumpCommand(Twengine::GameObject* gameObject);
	virtual void Execute() override;
	void IsPressBound() { m_IsPressBound = true; }
	void IsReleaseBound() { m_IsPressBound = false; }

private:
	bool m_IsPressBound{};
	DigDugComponent* m_DigDugComponent{};
};

