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
	auto& menuScene = Twengine::SceneManager::GetInstance().CreateScene("Menu");
	menuScene.SetLoadFunction(LevelFactory::LoadMenu);
	auto& level1Scene = Twengine::SceneManager::GetInstance().CreateScene("Level1");
	level1Scene.SetLoadFunction(LevelFactory::LoadLevel1);
	auto& level2Scene = Twengine::SceneManager::GetInstance().CreateScene("Level2");
	level2Scene.SetLoadFunction(LevelFactory::LoadLevel2);
	auto& level3Scene = Twengine::SceneManager::GetInstance().CreateScene("Level3");
	level3Scene.SetLoadFunction(LevelFactory::LoadLevel3);
	Twengine::SceneManager::GetInstance().SetCurrentScene("Level3");
}

int main(int, char* [])
{
	constexpr int MAX_STEPS_UP{ 5 };
	std::filesystem::path dataFolderName{ "Data" };
	int counter{ 0 };
	while (not std::filesystem::exists(dataFolderName) and counter < MAX_STEPS_UP)
	{
		std::filesystem::current_path(".."); // If "Data" Doesn't Exist In This Current Folder, Navigate One Up In The Hierarchy
		counter++;
	}
	std::filesystem::current_path(dataFolderName); // Set The Current Path, This Way The File System Paths Will Be Relative To This One And Thus To Data (If It Found A Data Folder)

	Twengine::Minigin engine{};
	engine.Run(load);
	return 0;
}