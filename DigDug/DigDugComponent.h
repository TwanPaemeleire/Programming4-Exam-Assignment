#pragma once
#include "Component.h"
#include "Observer.h"
#include <memory>
#include "PlayerStates.h"

namespace Twengine
{
	class AnimationComponent;
	class RectColliderComponent;
};

class DigDugComponent : public Twengine::Component, public Twengine::Observer
{
public:
	DigDugComponent(Twengine::GameObject* owner);
	virtual ~DigDugComponent() override = default;
	DigDugComponent(const DigDugComponent& other) = delete;
	DigDugComponent(DigDugComponent&& other) = delete;
	DigDugComponent& operator=(const DigDugComponent& other) = delete;
	DigDugComponent& operator=(DigDugComponent&& other) = delete;

	virtual void Start() override;
	virtual void Update() override;
	virtual void RenderUI() override;

	void SetDirection(glm::vec2 dir);
	virtual void Notify(const GameEvent& event, Twengine::GameObject* observedObject) override;

private:
	void CheckAndTransitionStates(std::unique_ptr<PlayerState> newState);
	std::unique_ptr<PlayerState> m_CurrentState;
	Twengine::AnimationComponent* m_AnimationComponent{};
	Twengine::RectColliderComponent* m_RectColliderComponent{};
};

