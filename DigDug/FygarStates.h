#pragma once
#include <memory>

namespace Twengine
{
	class GameObject;
	class AnimationComponent;
	class Event;
}

struct GameEvent;
class EnemyMovementComponent;
class GridComponent;
class RockComponent;
class FygarFireCommand;

class FygarState
{
public:
	FygarState() = default;
	virtual ~FygarState() = default;
	FygarState(const FygarState& other) = delete;
	FygarState(FygarState&& other) = delete;
	FygarState& operator=(const FygarState& other) = delete;
	FygarState& operator=(FygarState&& other) = delete;

	virtual void OnEnter(Twengine::GameObject*) {};
	virtual void OnExit(Twengine::GameObject*) {};
	virtual std::unique_ptr<FygarState> Update(Twengine::GameObject* stateOwner) = 0;
	virtual std::unique_ptr<FygarState> LateUpdate(Twengine::GameObject*) { return nullptr; }
	virtual std::unique_ptr<FygarState> GetNotifiedByOwner(const GameEvent&, Twengine::GameObject*, Twengine::GameObject*) { return nullptr; }
	virtual bool IsBeingPumped() const { return false; }
};

class FygarIdleState final : public FygarState
{
public:
	FygarIdleState() = default;
	virtual ~FygarIdleState() = default;
	FygarIdleState(const FygarIdleState& other) = delete;
	FygarIdleState(FygarIdleState&& other) = delete;
	FygarIdleState& operator=(const FygarIdleState& other) = delete;
	FygarIdleState& operator=(FygarIdleState&& other) = delete;

	virtual void OnEnter(Twengine::GameObject* stateOwner) override;
	virtual std::unique_ptr<FygarState> Update(Twengine::GameObject* stateOwner) override;
	virtual std::unique_ptr<FygarState> GetNotifiedByOwner(const GameEvent&, Twengine::GameObject*, Twengine::GameObject*) override;

private:
	EnemyMovementComponent* m_MovementComp{};
	bool m_PlayerControlled{false};
};

class FygarTrackingState final : public FygarState
{
public:
	FygarTrackingState() = default;
	virtual ~FygarTrackingState() = default;
	FygarTrackingState(const FygarTrackingState& other) = delete;
	FygarTrackingState(FygarTrackingState&& other) = delete;
	FygarTrackingState& operator=(const FygarTrackingState& other) = delete;
	FygarTrackingState& operator=(FygarTrackingState&& other) = delete;

	virtual void OnEnter(Twengine::GameObject* stateOwner) override;
	virtual std::unique_ptr<FygarState> Update(Twengine::GameObject* stateOwner) override;
	virtual std::unique_ptr<FygarState> GetNotifiedByOwner(const GameEvent&, Twengine::GameObject*, Twengine::GameObject*) override;

private:
	EnemyMovementComponent* m_MovementComp{};
	GridComponent* m_GridComponent{};
	const float m_DistanceToTriggerFire{86.f};
	const float m_FireCooldown{ 5.f };
	static float m_FireCooldownCounter;
	bool m_PlayerControlled{ false };
};

class FygarGhostState final : public FygarState
{
public:
	FygarGhostState() = default;
	virtual ~FygarGhostState() = default;
	FygarGhostState(const FygarGhostState& other) = delete;
	FygarGhostState(FygarGhostState&& other) = delete;
	FygarGhostState& operator=(const FygarGhostState& other) = delete;
	FygarGhostState& operator=(FygarGhostState&& other) = delete;

	virtual void OnEnter(Twengine::GameObject* stateOwner) override;
	virtual void OnExit(Twengine::GameObject* stateOwner) override;
	virtual std::unique_ptr<FygarState> Update(Twengine::GameObject* stateOwner) override;
	virtual std::unique_ptr<FygarState> GetNotifiedByOwner(const GameEvent&, Twengine::GameObject*, Twengine::GameObject*) override;

private:
	EnemyMovementComponent* m_MovementComp{};
	bool m_PlayerControlled{ false };
};

class FygarFireBreathingState final : public FygarState
{
public:
	FygarFireBreathingState() = default;
	virtual ~FygarFireBreathingState() = default;
	FygarFireBreathingState(const FygarFireBreathingState& other) = delete;
	FygarFireBreathingState(FygarFireBreathingState&& other) = delete;
	FygarFireBreathingState& operator=(const FygarFireBreathingState& other) = delete;
	FygarFireBreathingState& operator=(FygarFireBreathingState&& other) = delete;

	virtual void OnEnter(Twengine::GameObject* stateOwner) override;
	virtual std::unique_ptr<FygarState> Update(Twengine::GameObject* stateOwner) override;
	virtual std::unique_ptr<FygarState> LateUpdate(Twengine::GameObject* stateOwner) override;
	virtual std::unique_ptr<FygarState> GetNotifiedByOwner(const GameEvent&, Twengine::GameObject*, Twengine::GameObject*) override;

private:
	std::unique_ptr< FygarFireCommand> m_FireCommand{};
};

class FygarPumpingState final : public FygarState
{
public:
	FygarPumpingState() = default;
	virtual ~FygarPumpingState() = default;
	FygarPumpingState(const FygarPumpingState& other) = delete;
	FygarPumpingState(FygarPumpingState&& other) = delete;
	FygarPumpingState& operator=(const FygarPumpingState& other) = delete;
	FygarPumpingState& operator=(FygarPumpingState&& other) = delete;

	virtual void OnEnter(Twengine::GameObject* stateOwner) override;
	virtual std::unique_ptr<FygarState> Update(Twengine::GameObject* stateOwner) override;
	virtual std::unique_ptr<FygarState> GetNotifiedByOwner(const GameEvent&, Twengine::GameObject*, Twengine::GameObject*) override;
	virtual bool IsBeingPumped() const override { return true; }

private:
	Twengine::AnimationComponent* m_AnimationComponent{};

	const float m_DeflateDelay{ 0.8f };
	float m_DeflateDelayCounter{};
	bool m_IsBeingPumped{ true };
	bool m_HasBeenPumpedOnce{false};
};

class FygarRockDragState final : public FygarState
{
public:
	FygarRockDragState(RockComponent* rockComponent);
	virtual ~FygarRockDragState() = default;
	FygarRockDragState(const FygarRockDragState& other) = delete;
	FygarRockDragState(FygarRockDragState&& other) = delete;
	FygarRockDragState& operator=(const FygarRockDragState& other) = delete;
	FygarRockDragState& operator=(FygarRockDragState&& other) = delete;

	virtual void OnEnter(Twengine::GameObject* stateOwner) override;
	virtual std::unique_ptr<FygarState> Update(Twengine::GameObject* stateOwner) override;
private:
	Twengine::AnimationComponent* m_AnimationComponent{};
	RockComponent* m_RockComponent{};
	float m_AmountUnderRockToCheck{};
	std::unique_ptr<Twengine::Event> m_EnemyCrushedEvent{};
};

class FygarDeathState final : public FygarState
{
public:
	FygarDeathState() = default;
	virtual ~FygarDeathState() = default;
	FygarDeathState(const FygarDeathState& other) = delete;
	FygarDeathState(FygarDeathState&& other) = delete;
	FygarDeathState& operator=(const FygarDeathState& other) = delete;
	FygarDeathState& operator=(FygarDeathState&& other) = delete;

	virtual void OnEnter(Twengine::GameObject* stateOwner) override;
	virtual std::unique_ptr<FygarState> Update(Twengine::GameObject* stateOwner) override;
};