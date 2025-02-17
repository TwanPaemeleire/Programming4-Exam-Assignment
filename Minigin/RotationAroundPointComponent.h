#pragma once
#include "Component.h"
#include "glm.hpp"

class RotationAroundPointComponent final : public Component
{
public:
	RotationAroundPointComponent(GameObject* owner, TransformComponent* transform);
	virtual ~RotationAroundPointComponent() = default;
	RotationAroundPointComponent(const RotationAroundPointComponent& other) = delete;
	RotationAroundPointComponent(RotationAroundPointComponent&& other) = delete;
	RotationAroundPointComponent& operator=(const RotationAroundPointComponent& other) = delete;
	RotationAroundPointComponent& operator=(RotationAroundPointComponent&& other) = delete;

	virtual void Start() override;
	virtual void Update() override;

	void SetRotationPoint(const glm::vec3& point) { m_RotationPoint = point; }
	void SetRadius(float radius) { m_Radius = radius; }
	void SetSpeed(float speed) { m_Speed = speed; }

private:
	TransformComponent* m_OwnerTransform{};
	glm::vec3 m_RotationPoint{};
	float m_Radius{};
	float m_Speed{};
	float m_Angle{};

};

