#pragma once
#include "Component.h"
#include "RockStates.h"


class RockComponent final : public Twengine::Component
{
public:
	RockComponent(Twengine::GameObject* owner);
	virtual ~RockComponent() override = default;
	RockComponent(const RockComponent& other) = delete;
	RockComponent(RockComponent&& other) = delete;
	RockComponent& operator=(const RockComponent& other) = delete;
	RockComponent& operator=(RockComponent&& other) = delete;

	virtual void Start() override;
	virtual void Update() override;

private:

	void CheckAndTransitionStates(std::unique_ptr<RockState> newState);
	std::unique_ptr<RockState> m_CurrentState{};
};

