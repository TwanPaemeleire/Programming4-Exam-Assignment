#pragma once
#include "GameObjectCommand.h"
class HealthComponent;

class KillObjectCommand : public Twengine::GameObjectCommand
{
public:
	KillObjectCommand(Twengine::GameObject* gameObject);
	virtual void Execute() override;
private:
	HealthComponent* m_HealthComponent;
};

