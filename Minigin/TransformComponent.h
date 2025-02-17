#pragma once
#include <glm.hpp>

#include "Component.h"

class TransformComponent final : public Component
{
public:
	TransformComponent(GameObject* owner, TransformComponent* transform);
	virtual ~TransformComponent() override = default;
	TransformComponent(const TransformComponent& other) = delete;
	TransformComponent(TransformComponent&& other) = delete;
	TransformComponent& operator=(const TransformComponent& other) = delete;
	TransformComponent& operator=(TransformComponent&& other) = delete;

	virtual void Update() override {};

	const glm::vec3& GetPosition() const { return m_Position; }
	void SetPosition(float x, float y, float z);
private:
	glm::vec3 m_Position;
};
