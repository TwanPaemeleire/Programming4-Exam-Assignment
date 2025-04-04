#pragma once

namespace Twengine
{
	class Scene;
}

class LevelFactory
{
public:
	static void CreateAndAddDigDug(Twengine::Scene& scene);
	static void CreateAndAddPooka(Twengine::Scene& scene);
	static void CreateAndAddFygar(Twengine::Scene& scene);
	static void LoadLevelFromFile(Twengine::Scene& scene);
};

