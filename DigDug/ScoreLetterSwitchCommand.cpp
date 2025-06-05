#include "ScoreLetterSwitchCommand.h"
#include "ScoreSavingComponent.h"
#include "GameObject.h"

ScoreLetterSwitchCommand::ScoreLetterSwitchCommand(Twengine::GameObject* gameObject)
	:GameObjectCommand(gameObject)
{
	m_ScoreSavingComponent = gameObject->GetComponent<ScoreSavingComponent>();
}

void ScoreLetterSwitchCommand::Execute()
{
	m_ScoreSavingComponent->CycleLetterIndex(m_Direction);
}
