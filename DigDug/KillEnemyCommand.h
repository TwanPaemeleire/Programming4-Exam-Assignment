#pragma once
#include "GameObjectCommand.h"
#include <memory>

namespace Twengine
{
	class Event;
}

class KillEnemyCommand : public Twengine::GameObjectCommand
{
public:
	KillEnemyCommand(Twengine::GameObject* gameObject);
	virtual void Execute() override;

	Twengine::Event* GetEnemyKilledEvent() const { return m_EnemyKilledEvent.get(); }

private:
	std::unique_ptr<Twengine::Event> m_EnemyKilledEvent;
};

