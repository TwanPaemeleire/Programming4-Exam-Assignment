#pragma once
#include "Component.h"
#include "Observer.h"

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

	virtual void Notify(const GameEvent& event, Twengine::GameObject* observedObject) override;

private:
	double m_Angle{};

	Twengine::AnimationComponent* m_AnimationComponent{};
	Twengine::RectColliderComponent* m_RectColliderComponent{};
};

