#include "ScoreCharCycleCommand.h"
#include "ScoreSavingComponent.h"
#include "GameObject.h"

ScoreCharCycleCommand::ScoreCharCycleCommand(Twengine::GameObject* gameObject)
	:GameObjectCommand(gameObject)
{
	m_ScoreSavingComponent = gameObject->GetComponent<ScoreSavingComponent>();
}

void ScoreCharCycleCommand::Execute()
{
	m_ScoreSavingComponent->CycleCharacter(m_Direction);
}

