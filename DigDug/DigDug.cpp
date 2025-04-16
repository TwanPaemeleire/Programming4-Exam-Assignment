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
	Twengine::SceneManager::GetInstance().SetCurrentScene("Menu");
	auto& levelOneScene = Twengine::SceneManager::GetInstance().CreateScene("LevelOne");
	levelOneScene.SetLoadFunction(LevelFactory::LoadLevelOne);
	//Twengine::SceneManager::GetInstance().SetCurrentScene("LevelOne");
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