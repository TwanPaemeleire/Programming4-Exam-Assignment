#pragma once
#include "Component.h"
#include <memory>
#include "FygarStates.h"
#include "Event.h"
#include "Observer.h"

class FygarComponent final : public Twengine::Component, public Twengine::Observer
{
public:
	FygarComponent(Twengine::GameObject* owner);
	virtual ~FygarComponent() override = default;
	FygarComponent(const FygarComponent& other) = delete;
	FygarComponent(FygarComponent&& other) = delete;
	FygarComponent& operator=(const FygarComponent& other) = delete;
	FygarComponent& operator=(FygarComponent&& other) = delete;

	virtual void Start() override;
	virtual void Update() override;
	virtual void LateUpdate() override;

	void Notify(const GameEvent& event, Twengine::GameObject* observedObject) override;
	Twengine::Event* GetOnDeathEvent() const { return m_OnDeathEvent.get(); }

	void EnablePlayerControlled() { m_ControlledByPlayer = true; }
	bool IsPlayerControlled() const { return m_ControlledByPlayer; }

	bool IsBeingPumped() const { return m_CurrentState->IsBeingPumped(); }

private:
	void CheckAndTransitionStates(std::unique_ptr<FygarState> newState);
	std::unique_ptr<FygarState> m_CurrentState{};
	std::unique_ptr<Twengine::Event> m_OnDeathEvent{};
	bool m_ControlledByPlayer{false};
};

