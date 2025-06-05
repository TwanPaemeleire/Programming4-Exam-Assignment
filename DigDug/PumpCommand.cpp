#include "PumpCommand.h"
#include "DigDugComponent.h"
#include "GameObject.h"

PumpCommand::PumpCommand(Twengine::GameObject* gameObject)
	:GameObjectCommand(gameObject)
{
	m_DigDugComponent = gameObject->GetComponent<DigDugComponent>();
}

void PumpCommand::Execute()
{
	m_DigDugComponent->OnPumpButtonInteraction(m_InteractionState);
}
