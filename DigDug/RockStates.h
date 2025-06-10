#pragma once
#include <memory>
#include <glm.hpp>
#include <utility>
#include <vector>

namespace Twengine
{
	class GameObject;
	class RectColliderComponent;
	class AnimationComponent;
}

class GroundComponent;
class GridComponent;

class RockState
{
public:
	RockState() = default;
	virtual ~RockState() = default;
	RockState(const RockState& other) = delete;
	RockState(RockState&& other) = delete;
	RockState& operator=(const RockState& other) = delete;
	RockState& operator=(RockState&& other) = delete;

	virtual void OnEnter(Twengine::GameObject*) {};
	virtual void OnExit(Twengine::GameObject*) {};
	virtual std::unique_ptr<RockState> Update(Twengine::GameObject* stateOwner) = 0;
};

class RockIdleState final : public RockState
{
public:
	RockIdleState() = default;
	virtual ~RockIdleState() = default;
	RockIdleState(const RockIdleState& other) = delete;
	RockIdleState(RockIdleState&& other) = delete;
	RockIdleState& operator=(const RockIdleState& other) = delete;
	RockIdleState& operator=(RockIdleState&& other) = delete;

	virtual void OnEnter(Twengine::GameObject* stateOwner) override;
	virtual std::unique_ptr<RockState> Update(Twengine::GameObject* stateOwner) override;
	void OnExit(Twengine::GameObject*) override;

private:
	bool PlayerIsUnderRock();

	std::vector<Twengine::RectColliderComponent*> m_PlayerRectColliderComponents{};
	std::pair<int, int> m_IndexUnderRock{};
	glm::vec2 m_PositionToCheckIfFree{};
	bool m_PlayerIsBeneath{ false };
};

class RockFallingState final : public RockState
{
public:
	RockFallingState() = default;
	virtual ~RockFallingState() = default;
	RockFallingState(const RockFallingState& other) = delete;
	RockFallingState(RockFallingState&& other) = delete;
	RockFallingState& operator=(const RockFallingState& other) = delete;
	RockFallingState& operator=(RockFallingState&& other) = delete;

	virtual void OnEnter(Twengine::GameObject* stateOwner) override;
	virtual std::unique_ptr<RockState> Update(Twengine::GameObject* stateOwner) override;

private:
	GroundComponent* m_GroundComponent{};
	GridComponent* m_GridComponent{};
	const float m_DropSpeed{ 20.f };
	glm::vec2 m_Size{};
	bool m_HasLeftStartCell{ false };
	std::pair<int, int> m_StartCellIndex{};
};

class RockBreakingState final : public RockState
{
public:
	RockBreakingState() = default;
	virtual ~RockBreakingState() = default;
	RockBreakingState(const RockBreakingState& other) = delete;
	RockBreakingState(RockBreakingState&& other) = delete;
	RockBreakingState& operator=(const RockBreakingState& other) = delete;
	RockBreakingState& operator=(RockBreakingState&& other) = delete;

	virtual void OnEnter(Twengine::GameObject* stateOwner);
	virtual std::unique_ptr<RockState> Update(Twengine::GameObject* stateOwner) override;

private:
	Twengine::AnimationComponent* m_AnimationComponent;
};


