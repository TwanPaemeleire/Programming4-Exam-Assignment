#pragma once
#include <memory>
#include "Observer.h"

namespace Twengine
{
	class GameObject;
	class AnimationComponent;
}

class EnemyMovementComponent;
class DigDugPumpComponent;
struct GameEvent;

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
	virtual std::unique_ptr<PookaState> GetNotifiedByOwner(const GameEvent&, Twengine::GameObject*) { return nullptr; }
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

	virtual void OnEnter(Twengine::GameObject* stateOwner) override;
	virtual std::unique_ptr<PookaState> Update(Twengine::GameObject* stateOwner) override;

	virtual std::unique_ptr<PookaState> GetNotifiedByOwner(const GameEvent& event, Twengine::GameObject* observedObject) override;

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

	virtual void OnEnter(Twengine::GameObject* stateOwner) override;
	virtual std::unique_ptr<PookaState> Update(Twengine::GameObject* stateOwner) override;

	virtual std::unique_ptr<PookaState> GetNotifiedByOwner(const GameEvent& event, Twengine::GameObject* observedObject) override;

private:
	EnemyMovementComponent* m_MovementComp{};
};

class PookaGhostState final : public PookaState
{
public:
	PookaGhostState() = default;
	virtual ~PookaGhostState() = default;
	PookaGhostState(const PookaGhostState& other) = delete;
	PookaGhostState(PookaGhostState&& other) = delete;
	PookaGhostState& operator=(const PookaGhostState& other) = delete;
	PookaGhostState& operator=(PookaGhostState&& other) = delete;

	virtual void OnEnter(Twengine::GameObject* stateOwner) override;
	virtual void OnExit(Twengine::GameObject* stateOwner) override;
	virtual std::unique_ptr<PookaState> Update(Twengine::GameObject* stateOwner) override;
	virtual std::unique_ptr<PookaState> GetNotifiedByOwner(const GameEvent& event, Twengine::GameObject* observedObject) override;

private:
	EnemyMovementComponent* m_MovementComp{};
};

class PookaPumpingState final : public PookaState, public Twengine::Observer
{
public:
	PookaPumpingState(DigDugPumpComponent* digDugPumpComponent);
	virtual ~PookaPumpingState() = default;
	PookaPumpingState(const PookaPumpingState& other) = delete;
	PookaPumpingState(PookaPumpingState&& other) = delete;
	PookaPumpingState& operator=(const PookaPumpingState& other) = delete;
	PookaPumpingState& operator=(PookaPumpingState&& other) = delete;

	virtual void OnEnter(Twengine::GameObject* stateOwner) override;
	virtual std::unique_ptr<PookaState> Update(Twengine::GameObject* stateOwner) override;

	void Notify(const GameEvent& event, Twengine::GameObject* observedObject) override;
private:
	Twengine::AnimationComponent* m_AnimationComponent{};
	DigDugPumpComponent* m_DigDugPumpComponent;
};

class PookaDeathState final : public PookaState
{
public:
	PookaDeathState() = default;
	virtual ~PookaDeathState() = default;
	PookaDeathState(const PookaDeathState& other) = delete;
	PookaDeathState(PookaDeathState&& other) = delete;
	PookaDeathState& operator=(const PookaDeathState& other) = delete;
	PookaDeathState& operator=(PookaDeathState&& other) = delete;

	virtual void OnEnter(Twengine::GameObject* stateOwner) override;
	virtual std::unique_ptr<PookaState> Update(Twengine::GameObject* stateOwner) override;
};