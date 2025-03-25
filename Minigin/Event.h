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

		void NotifyObservers(const GameEvent& event, GameObject* observedObject);

	private:
		std::vector<Observer*> m_Observers;
	};
}
