#pragma once
#include "Component.h"
#include <glm.hpp>
#include <utility>

class GridComponent;

class DigDugMovementComponent final : public Twengine::Component
{
public:
	DigDugMovementComponent(Twengine::GameObject* owner);
	virtual ~DigDugMovementComponent() override = default;
	DigDugMovementComponent(const DigDugMovementComponent& other) = delete;
	DigDugMovementComponent(DigDugMovementComponent&& other) = delete;
	DigDugMovementComponent& operator=(const DigDugMovementComponent& other) = delete;
	DigDugMovementComponent& operator=(DigDugMovementComponent&& other) = delete;

	virtual void Start() override;
	virtual void Update() override;
	virtual void RenderUI() override;

	void SetXDirection(float x);
	void SetYDirection(float y);
private:
	void CalculateNextTarget();

	float m_MovementSpeed{20.f};
	GridComponent* m_GridComponent{};
	
	glm::vec2 m_Direction = { 0.f, 0.f }; 
	glm::vec2 m_TargetPosition = { -1.f, -1.f };
	glm::vec2 m_CurrentInputDirection = { 0.f, 0.f };
	float m_DistanceToTarget{};
	float m_DistanceTracker{};
	std::pair<int, int> m_CurrentIndex{};
	std::pair<int, int> m_NextIndex{};
	bool m_IsMoving = false;
	bool m_MovingHorizontally{ true };
	bool m_CanSwitchDirection{ true };
};

