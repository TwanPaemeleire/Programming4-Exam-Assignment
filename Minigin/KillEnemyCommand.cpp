#include "KillEnemyCommand.h"
#include "GameObject.h"
#include "Event.h"

KillEnemyCommand::KillEnemyCommand(Twengine::GameObject* gameObject)
	:GameObjectCommand(gameObject)
{
	m_EnemyKilledEvent = std::make_unique<Twengine::Event>();
}

void KillEnemyCommand::Execute()
{
	m_EnemyKilledEvent->NotifyObservers(GameEvent::EnemyKilled, GetGameObject());
}
