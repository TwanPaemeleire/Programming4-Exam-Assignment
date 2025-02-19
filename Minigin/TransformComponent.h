#pragma once
#include <glm.hpp>
#include "Component.h"

class TransformComponent final : public Component
{
public:
	TransformComponent(GameObject* owner);
	virtual ~TransformComponent() override = default;
	TransformComponent(const TransformComponent& other) = delete;
	TransformComponent(TransformComponent&& other) = delete;
	TransformComponent& operator=(const TransformComponent& other) = delete;
	TransformComponent& operator=(TransformComponent&& other) = delete;

	virtual void Update() override {};

	void SetLocalPosition(const glm::vec3& localPos);
	const glm::vec3& GetWorldPosition();
	void SetPositionDirty();

private:
	void UpdateWorldPosition();

	bool m_PositionIsDirty{ false };
	glm::vec3 m_WorldPosition{};
	glm::vec3 m_LocalPosition{};
};
