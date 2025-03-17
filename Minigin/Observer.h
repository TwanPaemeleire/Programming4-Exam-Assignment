#pragma once

class GameObject;
#include "GameEvents.h"

namespace Twengine
{
	class Observer
	{
	public:
		virtual ~Observer() = default;
		virtual void Notify(GameEvent event, Twengine::GameObject* observedObject) = 0;
	};
}