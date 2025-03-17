#pragma once
#include "Component.h"
#include "glm.hpp"

class RotationAroundPointComponent final : public Twengine::Component
{
public:
	RotationAroundPointComponent(Twengine::GameObject* owner);
	virtual ~RotationAroundPointComponent() = default;
	RotationAroundPointComponent(const RotationAroundPointComponent& other) = delete;
	RotationAroundPointComponent(RotationAroundPointComponent&& other) = delete;
	RotationAroundPointComponent& operator=(const RotationAroundPointComponent& other) = delete;
	RotationAroundPointComponent& operator=(RotationAroundPointComponent&& other) = delete;

	virtual void Update() override;

	void SetRadius(float radius) { m_Radius = radius; }
	void SetSpeed(float speed) { m_Speed = speed; }

private:
	float m_Radius{};
	float m_Speed{};
	float m_Angle{};
};

