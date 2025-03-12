#pragma once

class GameObject;
#include "GameEvents.h"

class Observer
{
public:
	virtual ~Observer() = default;
	virtual void Notify(GameEvent event, GameObject* observedObject) = 0;
};