#pragma once
#include <vector>
#include "GameEvent.h"

namespace Twengine
{
	class Observer;
	class GameObject;

	class Event final
	{
	public:
		void AddObserver(Observer* observer);
		void RemoveObserver(Observer* observer);
		size_t GetObserverCount() const { return m_Observers.size(); }

		void NotifyObservers(const GameEvent& event, GameObject* observedObject);

	private:
		std::vector<Observer*> m_Observers;
	};
}
