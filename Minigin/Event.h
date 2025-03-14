#pragma once
#include <vector>
#include "GameEvents.h"

class Observer;
class GameObject;

class Event final
{
public:
	void AddObserver(Observer* observer);
	void RemoveObserver(Observer* observer);

	void NotifyObservers(GameEvent event, GameObject* observedObject);

private:
	std::vector<Observer*> m_Observers;
};

