#pragma once
#include "GameObjectCommand.h"

class MainMenuComponent;

class ButtonSwitchCommand : public Twengine::GameObjectCommand
{
public:
	ButtonSwitchCommand(Twengine::GameObject* gameObject);
	virtual void Execute() override;
	void SetDirection(int dir) { m_Direction = dir; }

private:
	int m_Direction{1};
	MainMenuComponent* m_MainMenuComponent{};
};

