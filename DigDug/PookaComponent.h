#pragma once
#include "Component.h"
#include "glm.hpp"
#include <vector>
#include "Observer.h"

class GroundComponent;
class EnemyMovementComponent;

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
	virtual void Notify(const GameEvent& event, Twengine::GameObject* observedObject) override;

private:
	bool m_CanMoveToPlayer{ false };
	EnemyMovementComponent* m_MovementComponent;
};

