#include "KillEnemyCommand.h"
#include "GameObject.h"
#include "Event.h"
#include "ServiceLocator.h"
#include "SoundSystem.h"

KillEnemyCommand::KillEnemyCommand(Twengine::GameObject* gameObject)
	:GameObjectCommand(gameObject)
{
	m_EnemyKilledEvent = std::make_unique<Twengine::Event>();
	Twengine::ServiceLocator::get_sound_system().RequestLoadSound("DigDug/DigDugShot.wav", SoundId(make_sdbm_hash("DigDugShot")));
}

void KillEnemyCommand::Execute()
{
	m_EnemyKilledEvent->NotifyObservers(GameEvent(make_sdbm_hash("EnemyKilled")), GetGameObject());
	Twengine::ServiceLocator::get_sound_system().RequestPlaySound(SoundId(make_sdbm_hash("DigDugShot")), 0.9f);
}
