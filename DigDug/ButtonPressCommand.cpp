#include "ButtonPressCommand.h"
#include "GameObject.h"
#include "MainMenuComponent.h"

ButtonPressCommand::ButtonPressCommand(Twengine::GameObject* gameObject)	
	:GameObjectCommand(gameObject)
{
	m_MainMenuComponent = gameObject->GetComponent<MainMenuComponent>();
}

void ButtonPressCommand::Execute()
{
	m_MainMenuComponent->ButtonPressed();
}
