#include "ButtonSwitchCommand.h"
#include "MainMenuComponent.h"
#include "GameObject.h"

ButtonSwitchCommand::ButtonSwitchCommand(Twengine::GameObject* gameObject)
	:GameObjectCommand(gameObject)
{
	m_MainMenuComponent = gameObject->GetComponent<MainMenuComponent>();
}

void ButtonSwitchCommand::Execute()
{
	m_MainMenuComponent->ButtonSwitched(m_Direction);
}
