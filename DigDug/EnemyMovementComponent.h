#pragma once
#include "Component.h"
#include "glm.hpp"
#include <vector>
#include <memory>

class GroundComponent;
class GridComponent;

class EnemyMovementComponent final : public Twengine::Component
{
public:
	EnemyMovementComponent(Twengine::GameObject* owner);
	virtual ~EnemyMovementComponent() override = default;
	EnemyMovementComponent(const EnemyMovementComponent& other) = delete;
	EnemyMovementComponent(EnemyMovementComponent&& other) = delete;
	EnemyMovementComponent& operator=(const EnemyMovementComponent& other) = delete;
	EnemyMovementComponent& operator=(EnemyMovementComponent&& other) = delete;

	virtual void Start() override;
	virtual void RenderUI() override;

	bool MovementIfNoPathToPlayer();
	void SetNewIdleTarget();
	void SetMovementSpeed(float movementSpeed) { m_MovementSpeed = movementSpeed; }
	void PathFindingToPlayer();

private:
	glm::vec2 m_NextNodeToPlayer{ -1.f, -1.f };
	float m_MovementSpeed{ 20.f };

	// Idle Movement
	struct IdleTargetOption
	{
		glm::vec2 target{};
		glm::vec2 direction{};
		bool canMoveHere{};
	};

	glm::vec2 m_IdleTarget{};
	glm::vec2 m_IdleDirection{};
	int m_PreviousDirIndex{ -1 };
	float m_GridCellSize{};
	std::vector<IdleTargetOption> m_IdleDirections = std::vector<IdleTargetOption>(4);

	GroundComponent* m_GroundComponent{};
	GridComponent* m_GridComponent{};
};

