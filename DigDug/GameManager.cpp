#include "GameManager.h"
#include "SceneManager.h"

void GameManager::StartGameFromMenu(GameMode)
{
	Twengine::SceneManager::GetInstance().GetPersistentScene().ActivateAllObjects();
	Twengine::SceneManager::GetInstance().SetCurrentScene("Level1");
}
