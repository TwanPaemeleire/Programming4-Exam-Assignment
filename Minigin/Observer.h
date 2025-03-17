#pragma once
#include "GameEvents.h"

namespace Twengine
{
	class GameObject;

	class Observer
	{
	public:
		virtual ~Observer() = default;
		virtual void Notify(GameEvent event, GameObject* observedObject) = 0;
	};
}