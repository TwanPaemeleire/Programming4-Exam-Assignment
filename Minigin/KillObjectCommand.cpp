#include "KillObjectCommand.h"
#include "GameObject.h"
#include "HealthComponent.h"

KillObjectCommand::KillObjectCommand(GameObject* gameObject)
	:GameObjectCommand(gameObject)
{
	m_HealthComponent = gameObject->GetComponent<HealthComponent>();
}

void KillObjectCommand::Execute()
{
	m_HealthComponent->Kill();
}
