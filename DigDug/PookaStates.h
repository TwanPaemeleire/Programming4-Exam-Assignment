#pragma once
#include <memory>

namespace Twengine
{
	class GameObject;
}

class EnemyMovementComponent;

class PookaState
{
public:
	PookaState() = default;
	virtual ~PookaState() = default;
	PookaState(const PookaState& other) = delete;
	PookaState(PookaState&& other) = delete;
	PookaState& operator=(const PookaState& other) = delete;
	PookaState& operator=(PookaState&& other) = delete;

	virtual void OnEnter(Twengine::GameObject*) {};
	virtual void OnExit(Twengine::GameObject*) {};
	virtual std::unique_ptr<PookaState> Update(Twengine::GameObject* stateOwner) = 0;
	virtual void RenderDebugDrawing() const {};
};

class PookaIdleState final : public PookaState
{
public:
	PookaIdleState() = default;
	virtual ~PookaIdleState() = default;
	PookaIdleState(const PookaIdleState& other) = delete;
	PookaIdleState(PookaIdleState&& other) = delete;
	PookaIdleState& operator=(const PookaIdleState& other) = delete;
	PookaIdleState& operator=(PookaIdleState&& other) = delete;

	virtual void OnEnter(Twengine::GameObject*) override;
	virtual std::unique_ptr<PookaState> Update(Twengine::GameObject* stateOwner) override;

private:
	EnemyMovementComponent* m_MovementComp{};
};

class PookaTrackingState final : public PookaState
{
public:
	PookaTrackingState() = default;
	virtual ~PookaTrackingState() = default;
	PookaTrackingState(const PookaTrackingState& other) = delete;
	PookaTrackingState(PookaTrackingState&& other) = delete;
	PookaTrackingState& operator=(const PookaTrackingState& other) = delete;
	PookaTrackingState& operator=(PookaTrackingState&& other) = delete;

	virtual void OnEnter(Twengine::GameObject*) override;
	virtual std::unique_ptr<PookaState> Update(Twengine::GameObject* stateOwner) override;

private:
	EnemyMovementComponent* m_MovementComp{};
};

