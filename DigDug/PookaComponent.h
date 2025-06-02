#pragma once
#include "Component.h"
#include <memory>
#include "PookaStates.h"
#include "Observer.h"
#include "Event.h"

class PookaComponent final : public Twengine::Component, public Twengine::Observer
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

	void Notify(const GameEvent& event, Twengine::GameObject* observedObject) override;
	Twengine::Event* GetOnDeathEvent() const { return m_OnDeathEvent.get(); }
private:
	void CheckAndTransitionStates(std::unique_ptr<PookaState> newState);
	std::unique_ptr<PookaState> m_CurrentState{};
	std::unique_ptr<Twengine::Event> m_OnDeathEvent{};
};

