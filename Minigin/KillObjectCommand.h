#pragma once
#include "GameObjectCommand.h"
class HealthComponent;

class KillObjectCommand : public GameObjectCommand
{
public:
	KillObjectCommand(GameObject* gameObject);
	virtual void Execute() override;
private:
	HealthComponent* m_HealthComponent;
};

