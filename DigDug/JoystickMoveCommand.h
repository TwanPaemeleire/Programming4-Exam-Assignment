#pragma once
#include "JoystickCommand.h"

namespace Twengine
{
	class AnimatedTextureComponent;
}

class JoystickMoveCommand final : public Twengine::JoystickCommand
{
public:
	JoystickMoveCommand(Twengine::GameObject* gameObject);
	virtual void Execute(float x, float y) override;

private:
	Twengine::AnimatedTextureComponent* m_AnimationComponent;
	float m_Speed{ 20.f };
};

