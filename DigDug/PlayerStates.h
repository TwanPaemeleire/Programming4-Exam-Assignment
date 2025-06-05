#pragma once
#include <memory>
#include <glm.hpp>
#include "Observer.h"
#include "InputManager.h"

namespace Twengine
{
	class GameObject;
	class AnimationComponent;
	class TransformComponent;
	//struct InteractionStates;
}
class GroundComponent;
class GridComponent;
class DigDugPumpComponent;

class PlayerState
{
public:
	PlayerState() = default;
	virtual ~PlayerState() = default;
	PlayerState(const PlayerState& other) = delete;
	PlayerState(PlayerState&& other) = delete;
	PlayerState& operator=(const PlayerState& other) = delete;
	PlayerState& operator=(PlayerState&& other) = delete;

	virtual void OnEnter(Twengine::GameObject*) {};
	virtual void OnExit(Twengine::GameObject*) {};
	virtual std::unique_ptr<PlayerState> Update(Twengine::GameObject* stateOwner) =0;
	virtual void RenderDebugDrawing() const {};
	virtual std::unique_ptr<PlayerState> SetXDirection(Twengine::GameObject*, float, float) { return nullptr; }
	virtual std::unique_ptr<PlayerState> SetYDirection(Twengine::GameObject*, float, float) { return nullptr; }
	virtual std::unique_ptr<PlayerState> OnPumpButtonInteraction(Twengine::GameObject*, Twengine::InteractionStates) { return nullptr; }
	virtual std::unique_ptr<PlayerState> Notify(Twengine::GameObject*, const GameEvent&) { return nullptr; }
};

class PlayerMoving final : public PlayerState
{
public:
	PlayerMoving() = default;
	virtual ~PlayerMoving() override = default;
	PlayerMoving(const PlayerMoving& other) = delete;
	PlayerMoving(PlayerMoving&& other) = delete;
	PlayerMoving& operator=(const PlayerMoving& other) = delete;
	PlayerMoving& operator=(PlayerMoving&& other) = delete;

	virtual void OnEnter(Twengine::GameObject* stateOwner) override;
	virtual std::unique_ptr<PlayerState> Update(Twengine::GameObject* stateOwner) override;
	virtual void OnExit(Twengine::GameObject* stateOwner) override;
	virtual void RenderDebugDrawing() const override;
	virtual std::unique_ptr<PlayerState> SetXDirection(Twengine::GameObject* stateOwner, float x, float yToSet) override;
	virtual std::unique_ptr<PlayerState> SetYDirection(Twengine::GameObject* stateOwner, float y, float xToSet) override;
	virtual std::unique_ptr<PlayerState> OnPumpButtonInteraction(Twengine::GameObject* stateOwner, Twengine::InteractionStates) override;
	virtual std::unique_ptr<PlayerState> Notify(Twengine::GameObject*, const GameEvent&) override;
private:
	void CalculateNextTarget();
	void SetIdleAnimation();
	void UpdateGroundAndAnimation();
	void UpdateFlipAndRotation();

	float m_MovementSpeed{ 20.f };
	GridComponent* m_GridComponent{};
	GroundComponent* m_GroundComponent{};
	Twengine::AnimationComponent* m_AnimationComponent{};
	Twengine::TransformComponent* m_Transform{};

	glm::vec2 m_Direction = { 0.f, 0.f };
	glm::vec2 m_LastNonNullDirection;
	glm::vec2 m_TargetPosition = { -1.f, -1.f };
	glm::vec2 m_CurrentInputDirection = { 0.f, 0.f };
	float m_DistanceToTarget{};
	float m_DistanceTracker{};
	std::pair<int, int> m_CurrentIndex{};
	bool m_IsMoving{ false };
	bool m_ShouldStartPumping{ false };

	bool m_HasStartedDiggingAnimation{ false };
	bool m_HasStartedWalkingAnimation{ false };
	bool m_HasStartedIdleAnimation{ false };
};

class PlayerPumpingState final : public PlayerState
{
public:
	PlayerPumpingState(const glm::vec2& facingDir);
	virtual ~PlayerPumpingState() = default;
	PlayerPumpingState(const PlayerPumpingState& other) = delete;
	PlayerPumpingState(PlayerPumpingState&& other) = delete;
	PlayerPumpingState& operator=(const PlayerPumpingState& other) = delete;
	PlayerPumpingState& operator=(PlayerPumpingState&& other) = delete;

	virtual void OnEnter(Twengine::GameObject*) override;
	virtual std::unique_ptr<PlayerState> Update(Twengine::GameObject* stateOwner) override;
	virtual std::unique_ptr<PlayerState> OnPumpButtonInteraction(Twengine::GameObject* stateOwner, Twengine::InteractionStates) override;

	virtual std::unique_ptr<PlayerState> Notify(Twengine::GameObject*, const GameEvent&) override;
private:
	void SetPositionAndDirectionOfPump(Twengine::GameObject* stateOwner, Twengine::GameObject* pumpObject, float frameWidth, float frameHeight);

	DigDugPumpComponent* m_DigDugPumpComponent{};
	glm::vec2 m_FacingDirection{};
};

class PlayerDeathState final : public PlayerState
{
public:
	PlayerDeathState() = default;
	virtual ~PlayerDeathState() = default;
	PlayerDeathState(const PlayerDeathState& other) = delete;
	PlayerDeathState(PlayerDeathState&& other) = delete;
	PlayerDeathState& operator=(const PlayerDeathState& other) = delete;
	PlayerDeathState& operator=(PlayerDeathState&& other) = delete;

	virtual void OnEnter(Twengine::GameObject*) override;
	virtual std::unique_ptr<PlayerState> Update(Twengine::GameObject* stateOwner) override;

private:
	Twengine::AnimationComponent* m_AnimationComponent{};
};

