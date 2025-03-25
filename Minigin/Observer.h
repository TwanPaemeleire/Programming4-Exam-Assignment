#pragma once
#include "GameEvent.h"

namespace Twengine
{
	class GameObject;

	class Observer
	{
	public:
		virtual ~Observer() = default;
		virtual void Notify(const GameEvent& event, GameObject* observedObject) = 0;
	};
}