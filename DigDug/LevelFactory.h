#pragma once
#include <string>

namespace Twengine
{
	class Scene;
}

class GroundComponent;

class LevelFactory
{
public:
	static void LoadMenu();
	static void LoadLevel1();
	static void LoadLevel2();
	static void LoadLevel3();

private:
	static void CreateAndAddDigDug(Twengine::Scene& scene);
	static void CreateAndAddPooka(Twengine::Scene& scene);
	static void CreateAndAddFygar(Twengine::Scene& scene);
	static void LoadLevelFromFile(GroundComponent* groundComponent, const std::string& filePath);
};

