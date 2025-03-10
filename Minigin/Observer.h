#pragma once

class GameObject;
struct Event;

class Observer
{
public:
	virtual ~Observer() = default;
	virtual void Notify(Event event, GameObject* observedObject) = 0;
};

