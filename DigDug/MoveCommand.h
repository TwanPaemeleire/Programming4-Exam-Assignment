#pragma once
#include "GameObjectCommand.h"
#include "glm.hpp"

class MoveCommand : public Twengine::GameObjectCommand
{
public:
	MoveCommand(Twengine::GameObject* gameObject);
	virtual void Execute() override;

	void SetDirection(glm::vec2 direction) { m_Direction = direction; }
	void SetDirection(float x, float y) { SetDirection(glm::vec2(x, y)); }

private:
	glm::vec2 m_Direction{};
};

