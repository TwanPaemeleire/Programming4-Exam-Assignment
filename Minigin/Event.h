#pragma once
#include <vector>
#include "GameEvents.h"


namespace Twengine
{
	class Observer;
	class GameObject;
	class Event final
	{
	public:
		void AddObserver(Twengine::Observer* observer);
		void RemoveObserver(Twengine::Observer* observer);

		void NotifyObservers(GameEvent event, Twengine::GameObject* observedObject);

	private:
		std::vector<Twengine::Observer*> m_Observers;
	};
}
