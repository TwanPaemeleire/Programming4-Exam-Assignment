#if _DEBUG
#if __has_include(<vld.h>)
#include <vld.h>
#endif
#endif

#include <iostream>
#include "Minigin.h"
#include "SceneManager.h"
#include <filesystem>
#include "LevelFactory.h"

void load()
{

	Twengine::SceneManager::GetInstance().CreateScene("PersistentScene", LevelFactory::LoadPersistentScene, true).DeactivateAllObjects();
	Twengine::SceneManager::GetInstance().CreateScene("Level1", LevelFactory::LoadLevel1);
	Twengine::SceneManager::GetInstance().CreateScene("MainMenu", LevelFactory::LoadMainMenu);
	Twengine::SceneManager::GetInstance().CreateScene("HighScoreScene", LevelFactory::LoadHighScoreScene);
	//Twengine::SceneManager::GetInstance().CreateScene("Level2", LevelFactory::LoadLevel2);
	//Twengine::SceneManager::GetInstance().CreateScene("Level3", LevelFactory::LoadLevel3);
	Twengine::SceneManager::GetInstance().SetCurrentScene("MainMenu");
}

int main(int, char* [])
{
	constexpr int MAX_STEPS_UP{ 5 };
	std::filesystem::path dataFolderName{ "Data" };
	int counter{ 0 };
	while (not std::filesystem::exists(dataFolderName) and counter < MAX_STEPS_UP)
	{
		std::filesystem::current_path(".."); // If "Data" doesn't exist in this current folder, navigate one up in the hierarchy
		counter++;
	}
	std::filesystem::current_path(dataFolderName); // Set the current path, this way the file system paths will be relative to this one and thus to "Data" (if it found a "Data" folder)

	Twengine::Minigin engine{};
	engine.Run(load);
	return 0;
}