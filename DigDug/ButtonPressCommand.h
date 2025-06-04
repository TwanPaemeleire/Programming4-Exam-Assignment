#pragma once
#include "GameObjectCommand.h"

class MainMenuComponent;

class ButtonPressCommand : public Twengine::GameObjectCommand
{
public:
	ButtonPressCommand(Twengine::GameObject* gameObject);
	virtual void Execute() override;

private:
	MainMenuComponent* m_MainMenuComponent{};
};

