#pragma once
#include "Component.h"
#include "glm.hpp"
#include <vector>
#include <memory>

namespace Twengine
{
	class RectColliderComponent;
}

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
	virtual void Update() override;
	virtual void RenderUI() override;

	bool MovementIfNoPathToPlayer();
	bool MovementInGhostForm();
	void SetNewIdleTarget();
	void SetMovementSpeed(float movementSpeed) { m_MovementSpeed = movementSpeed; }
	void PathFindingToPlayer();

	bool GhostCoolDownHasFinished() const { return m_GhostCooldownHasFinished; }
	void ResetGhostStateValues();

private:
	// Ghost movement
	float m_MinimumGhostFormCooldown{20.f};
	float m_MaximumGhostFormCooldown{40.f};
	float m_GhostCooldown{};
	float m_GhostCoolDownTimer{};
	bool m_GhostCooldownHasFinished{false};

	float m_MinimumTimeInGhostForm{2.f};
	float m_GhostFormTimer{ 0.f };
	glm::vec2 m_CellToPositionIn{};
	bool m_IsGettingPositionedInCell{ false };

	// Pathfinding movement
	glm::vec2 m_NextNodeToPlayer{ -1.f, -1.f };
	float m_MovementSpeed{ 20.f };

	// Idle movement
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
	Twengine::RectColliderComponent* m_RectColliderComponent{};
	float m_HalfGridCellSize{};
};

