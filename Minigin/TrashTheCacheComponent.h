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
	Transform transform;
public:
	int ID;
};

class GameObject3DAlt
{
	Transform* transform;
public:
	int ID;
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

	virtual void Start() override;
	virtual void Update() override;
	virtual void Render() const override;
private:
	void Exercise1() const;
	void DrawExercise1Graph() const;

	void Exercise2GameObject3D() const;
	void DrawExercise2GameObject3DGraph() const;
	void Exercise2GameObject3DAlt() const;
	void DrawExercise2GameObject3DAltGraph() const;
	void DrawExercise2Combined() const;

	mutable int m_SampleCount{ 10 };
	mutable bool m_ButtonExercise1Pressed{ false };

	int m_SizeIntArray;
	int m_SizeGameObject3DArray; // Needs Different Size, As Otherwise An Error Occurs
	int* m_IntArray;
	GameObject3D* m_GameObject3DArray;
	GameObject3DAlt* m_GameObject3DAltArray;

	float* m_StepSizes;
	const int m_AmountOfSteps{ 11 };

	float* m_StepTimingsInt;
	float* m_StepTimingsGameObject3D;
	float* m_StepTimingsGameObject3DAlt;
};

