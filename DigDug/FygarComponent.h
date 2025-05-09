#pragma once
#include "Component.h"
#include <memory>
#include "FygarStates.h"

class FygarComponent final : public Twengine::Component
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

private:
	void CheckAndTransitionStates(std::unique_ptr<FygarState> newState);
	std::unique_ptr<FygarState> m_CurrentState{};
};

