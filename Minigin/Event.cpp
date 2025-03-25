#include "Event.h"
#include "GameObject.h"
#include "Observer.h"

void Twengine::Event::AddObserver(Observer* observer)
{
	m_Observers.push_back(observer);
}

void Twengine::Event::RemoveObserver(Observer* observer)
{
	m_Observers.erase(std::remove(m_Observers.begin(), m_Observers.end(), observer), m_Observers.end());
}

void Twengine::Event::NotifyObservers(const GameEvent& event, GameObject* observedObject)
{
	for (auto observer : m_Observers)
	{
		observer->Notify(event, observedObject);
	}
}
