#pragma once
#include "Component.h"
#include "glm.hpp"
#include <vector>
#include "Observer.h"

class GroundComponent;
class EnemyMovementComponent;

class FygarComponent final : public Twengine::Component, public Twengine::Observer
{
public:
	FygarComponent(Twengine::GameObject* owner);
	virtual ~FygarComponent() override = default;
	FygarComponent(const FygarComponent& other) = delete;
	FygarComponent(FygarComponent&& other) = delete;
	FygarComponent& operator=(const FygarComponent& other) = delete;
	FygarComponent& operator=(FygarComponent&& other) = delete;

	virtual void Update() override;
	virtual void Notify(const GameEvent& event, Twengine::GameObject* observedObject) override;

private:
	bool m_CanMoveToPlayer{ false };
	EnemyMovementComponent* m_MovementComponent;
};

