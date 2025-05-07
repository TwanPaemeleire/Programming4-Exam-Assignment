#pragma once
#include "Component.h"
#include <memory>
#include "PookaStates.h"

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

private:
	void CheckAndTransitionStates(std::unique_ptr<PookaState> newState);
	std::unique_ptr<PookaState> m_CurrentState{};
};

