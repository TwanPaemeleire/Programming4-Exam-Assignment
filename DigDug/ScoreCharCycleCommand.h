#pragma once
#include "GameObjectCommand.h"

class ScoreSavingComponent;

class ScoreCharCycleCommand : public Twengine::GameObjectCommand
{
public:
	ScoreCharCycleCommand(Twengine::GameObject* gameObject);
	virtual void Execute() override;
	void SetDirection(int dir) { m_Direction = dir; }

private:
	int m_Direction{ 1 };
	ScoreSavingComponent* m_ScoreSavingComponent{};
};



