#pragma once
#include <memory>

namespace Twengine
{
	class GameObject;
}

class EnemyMovementComponent;

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
	virtual void RenderDebugDrawing() const {};
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

private:
	EnemyMovementComponent* m_MovementComp{};
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

private:
	EnemyMovementComponent* m_MovementComp{};
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

private:
	EnemyMovementComponent* m_MovementComp{};
};
