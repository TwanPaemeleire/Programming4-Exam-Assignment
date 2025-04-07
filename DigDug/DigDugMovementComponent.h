#pragma once
#include "Component.h"
#include <glm.hpp>

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

	void SetXDirection(float x);
	void SetYDirection(float y);
private:
	glm::vec2 m_DesiredDirection{};
	glm::vec2 m_Direction{};
	float m_MovementSpeed{20.f};
	GridComponent* m_GridComponent;
};

