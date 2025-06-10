#include "GoToNextLevelCommand.h"
#include "GameManager.h"

GoToNextLevelCommand::GoToNextLevelCommand(Twengine::GameObject* gameObject)
	:GameObjectCommand(gameObject)
{
}

void GoToNextLevelCommand::Execute()
{
	GameManager::GetInstance().Notify(GameEvent(make_sdbm_hash("GoToNextLevel")), GetGameObject());
}
