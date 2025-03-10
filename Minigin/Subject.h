#pragma once
#include <vector>

class Observer;
class GameObject;
struct Event;

class Subject
{
public:
	void AddObserver(Observer* observer);
	void RemoveObserver(Observer* observer);

	void NotifyObservers(Event event, GameObject observedObject);

private:
	std::vector<Observer*> m_Observers;
};

