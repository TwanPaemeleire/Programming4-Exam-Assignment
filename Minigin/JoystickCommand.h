#pragma once
#include "GameObjectCommand.h"
namespace Twengine
{
    class JoystickCommand : public GameObjectCommand
    {
    public:
        JoystickCommand(GameObject* gameObject);
        virtual void Execute(float x, float y) = 0;
        virtual void Execute() override {};
    };
}