#pragma once
#include <glm.hpp>

#include "Component.h"

class TransformComponent final : public Component
{
public:
	const glm::vec3& GetPosition() const { return m_position; }
	void SetPosition(float x, float y, float z);
private:
	glm::vec3 m_position;
};
