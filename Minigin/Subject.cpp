#include "Subject.h"
#include "GameObject.h"
#include "Observer.h"

void Subject::AddObserver(Observer* observer)
{
	m_Observers.push_back(observer);
}

void Subject::RemoveObserver(Observer* observer)
{
	m_Observers.erase(std::remove(m_Observers.begin(), m_Observers.end(), observer), m_Observers.end());
}

void Subject::NotifyObservers(GameEvent event, GameObject* observedObject)
{
	for (auto observer : m_Observers)
	{
		observer->Notify(event, observedObject);
	}
}
