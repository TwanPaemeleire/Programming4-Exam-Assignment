#pragma once
#include "Component.h"

class DigDugComponent : public Twengine::Component
{
public:
	DigDugComponent(Twengine::GameObject* owner);
	virtual ~DigDugComponent() override = default;
	DigDugComponent(const DigDugComponent& other) = delete;
	DigDugComponent(DigDugComponent&& other) = delete;
	DigDugComponent& operator=(const DigDugComponent& other) = delete;
	DigDugComponent& operator=(DigDugComponent&& other) = delete;

	virtual void Update() override;

private:
	float m_FlipCounter{ 0.f };
	float m_FlipDelay{ 2.f };
	bool m_FlipHorizontal{ false };
};

