#pragma once
#include "Component.h"
#include "glm.hpp"
#include <vector>

class GroundComponent;
class GridComponent;

class PookaComponent final : public Twengine::Component
{
public:
	PookaComponent(Twengine::GameObject* owner);
	virtual ~PookaComponent() override = default;
	PookaComponent(const PookaComponent& other) = delete;
	PookaComponent(PookaComponent&& other) = delete;
	PookaComponent& operator=(const PookaComponent& other) = delete;
	PookaComponent& operator=(PookaComponent&& other) = delete;

	virtual void Start() override;
	virtual void Update() override;
	virtual void RenderUI() override;

private:
	// Pathfinding To Player
	glm::vec2 m_NextNodeToPlayer{-1.f, -1.f};
	float m_Width{};
	float m_Height{};
	float m_MovementSpeed{20.f};

	// Idle Movement
	struct IdleTargetOption
	{
		glm::vec2 target{};
		glm::vec2 direction{};
		bool canMoveHere{};
	};

	void MovementIfNoPathToPlayer();
	void SetNewIdleTarget();
	glm::vec2 m_IdleTarget{};
	glm::vec2 m_IdleDirection{};
	int m_PreviousDirIndex{-1};
	float m_GridCellSize{};
	std::vector<IdleTargetOption> m_IdleDirections = std::vector<IdleTargetOption>(4);

	GroundComponent* m_GroundComponent{};
	GridComponent* m_GridComponent{};
};

