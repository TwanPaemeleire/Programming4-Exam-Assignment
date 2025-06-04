#pragma once
#include "GameObjectCommand.h"
class LivesComponent;

class KillObjectCommand : public Twengine::GameObjectCommand
{
public:
	KillObjectCommand(Twengine::GameObject* gameObject);
	virtual void Execute() override;
private:
	LivesComponent* m_HealthComponent;
};

