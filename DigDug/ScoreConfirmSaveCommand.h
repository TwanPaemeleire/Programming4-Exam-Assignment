#pragma once
#include "GameObjectCommand.h"

class ScoreSavingComponent;

class ScoreConfirmSaveCommand : public Twengine::GameObjectCommand
{
public:
	ScoreConfirmSaveCommand(Twengine::GameObject* gameObject);
	virtual void Execute() override;

private:
	ScoreSavingComponent* m_ScoreSavingComponent{};
};
