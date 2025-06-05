#include "ScoreConfirmSaveCommand.h"
#include "ScoreSavingComponent.h"
#include "GameObject.h"

ScoreConfirmSaveCommand::ScoreConfirmSaveCommand(Twengine::GameObject* gameObject)
	:GameObjectCommand(gameObject)
{
	m_ScoreSavingComponent = gameObject->GetComponent<ScoreSavingComponent>();
}

void ScoreConfirmSaveCommand::Execute()
{
	m_ScoreSavingComponent->ConfirmSave();
}
