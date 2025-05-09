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
	auto& devScene = Twengine::SceneManager::GetInstance().CreateScene("DevScene");
	devScene.SetLoadFunction(LevelFactory::LoadDevScene);
	auto& mainMenuScene = Twengine::SceneManager::GetInstance().CreateScene("MainMenu");
	mainMenuScene.SetLoadFunction(LevelFactory::LoadMainMenu);
	auto& level1Scene = Twengine::SceneManager::GetInstance().CreateScene("Level1");
	level1Scene.SetLoadFunction(LevelFactory::LoadLevel1);
	auto& level2Scene = Twengine::SceneManager::GetInstance().CreateScene("Level2");
	level2Scene.SetLoadFunction(LevelFactory::LoadLevel2);
	auto& level3Scene = Twengine::SceneManager::GetInstance().CreateScene("Level3");
	level3Scene.SetLoadFunction(LevelFactory::LoadLevel3);
	Twengine::SceneManager::GetInstance().SetCurrentScene("DevScene");
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