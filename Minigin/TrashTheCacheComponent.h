#pragma once
#include "Component.h"
#include <vec2.hpp>

struct Transform
{
	float matrix[16] = {
		1,0,0,0,
		0,1,0,0,
		0,0,1,0,
		0,0,0,1 };
};

class GameObject3D
{
	Transform transform{};
public:
	int ID{};
};

class GameObject3DAlt
{
	Transform* transform{};
public:
	int ID{};
};

class TrashTheCacheComponent: public Component
{
public:

	TrashTheCacheComponent(GameObject* owner);
	virtual ~TrashTheCacheComponent() override;
	TrashTheCacheComponent(const TrashTheCacheComponent& other) = delete;
	TrashTheCacheComponent(TrashTheCacheComponent&& other) = delete;
	TrashTheCacheComponent& operator=(const TrashTheCacheComponent& other) = delete;
	TrashTheCacheComponent& operator=(TrashTheCacheComponent&& other) = delete;

	virtual void Update() override;
private:
	void CalculateIntTimings();
	void DrawExercise1Graph() const;

	void CalculateGameObject3DTimings();
	void DrawGameObject3DGraph() const;
	void CalculateGameObject3DAltTimings();
	void DrawGameObject3DAltGraph() const;

	void DrawExercise2CombinedGraph() const;

	int m_SampleCountInt{ 10 };
	int m_SampleCountGameObject3D{ 10 };

	int m_SizeIntArray;
	int m_SizeGameObject3DArray; // Needs Different Size, As Otherwise Too Much Memory Gets Allocated And An Error Will Occur
	int* m_IntArray;
	GameObject3D* m_GameObject3DArray;
	GameObject3DAlt* m_GameObject3DAltArray;

	static constexpr int m_AmountOfSteps{ 11 };
	float m_StepSizes[m_AmountOfSteps]{ 1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024 };

	float m_StepTimingsInt[m_AmountOfSteps]{};
	float m_StepTimingsGameObject3D[m_AmountOfSteps]{};
	float m_StepTimingsGameObject3DAlt[m_AmountOfSteps]{};

	bool m_IntButtonClicked{ false };
	bool m_GameObject3DButtonClicked{ false };
	bool m_GameObject3DAltButtonClicked{ false };

	bool m_ShowIntGraph{ false };
	bool m_ShowGameObject3DGraph{ false };
	bool m_ShowGameObject3DAltGraph{ false };

	unsigned int m_IntGraphColor{};
	unsigned int m_GameObjectColors[2]{};
};

