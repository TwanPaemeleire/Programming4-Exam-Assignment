#include "KillEnemyCommand.h"
#include "GameObject.h"
#include "Event.h"
#include "ServiceLocator.h"
#include "SoundSystem.h"

KillEnemyCommand::KillEnemyCommand(Twengine::GameObject* gameObject)
	:GameObjectCommand(gameObject)
{
	m_EnemyKilledEvent = std::make_unique<Twengine::Event>();
}

void KillEnemyCommand::Execute()
{
	m_EnemyKilledEvent->NotifyObservers(GameEvent(make_sdbm_hash("EnemyKilled")), GetGameObject());
	Twengine::ServiceLocator::get_sound_system().RequestPlaySound("DigDug/DigDugShot.wav", 0.9f);
}
