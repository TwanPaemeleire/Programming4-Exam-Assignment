#include "LevelFactory.h"
#include "GameObject.h"
#include "Scene.h"

void LevelFactory::CreateAndAddDigDug(Twengine::Scene& scene)
{
	std::unique_ptr<Twengine::GameObject> digDug = std::make_unique<Twengine::GameObject>();
	scene.Add(std::move(digDug));
}

void LevelFactory::CreateAndAddPooka(Twengine::Scene& scene)
{
	std::unique_ptr<Twengine::GameObject> pooka = std::make_unique<Twengine::GameObject>();
	scene.Add(std::move(pooka));
}

void LevelFactory::CreateAndAddFygar(Twengine::Scene& scene)
{
	std::unique_ptr<Twengine::GameObject> fygar = std::make_unique<Twengine::GameObject>();
	scene.Add(std::move(fygar));
}

void LevelFactory::LoadLevelFromFile(Twengine::Scene& scene)
{
	scene;
}
