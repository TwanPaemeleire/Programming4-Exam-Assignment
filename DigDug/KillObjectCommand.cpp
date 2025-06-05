#include "KillObjectCommand.h"
#include "GameObject.h"
#include "LivesComponent.h"

KillObjectCommand::KillObjectCommand(Twengine::GameObject* gameObject)
	:GameObjectCommand(gameObject)
{
	m_HealthComponent = gameObject->GetComponent<LivesComponent>();
}

void KillObjectCommand::Execute()
{
	m_HealthComponent->Kill();
}
