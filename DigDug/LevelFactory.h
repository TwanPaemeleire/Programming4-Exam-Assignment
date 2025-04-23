#pragma once
#include <string>

namespace Twengine
{
	class Scene;
}

class GroundComponent;
class GridComponent;

class LevelFactory
{
public:
	static void LoadDevScene();
	static void LoadMainMenu();
	static void LoadLevel1();
	static void LoadLevel2();
	static void LoadLevel3();

private:
	static void LoadLevelFromFile(Twengine::Scene& scene, GroundComponent* groundComponent, GridComponent* gridComponent, const std::string& filePath);
	static void CreateAndAddPooka(Twengine::Scene& scene, int row, int column, GridComponent* gridComponent);
	static void CreateAndAddFygar(Twengine::Scene& scene, int row, int column, GridComponent* gridComponent);
	static void CreateAndAddRock(Twengine::Scene& scene, int row, int column, GridComponent* gridComponent);
};

