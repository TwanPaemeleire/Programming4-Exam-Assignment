#include "ScoreComponent.h"
#include "Event.h"

ScoreComponent::ScoreComponent(Twengine::GameObject* owner)
	:Twengine::Component(owner)
{
	m_ScoreChangedEvent = std::make_unique<Twengine::Event>();
}

void ScoreComponent::SetScore(int points)
{
	m_Score = points;
	m_ScoreChangedEvent->NotifyObservers(GameEvent::ScoreChanged, GetOwner());
}

void ScoreComponent::Notify(GameEvent event, Twengine::GameObject* observedObject)
{
	// Use observed object to get it's layer or something?
	observedObject;
	if (event == GameEvent::PlayerDied)
	{
		m_Score -= 200;
		m_ScoreChangedEvent->NotifyObservers(GameEvent::ScoreChanged, GetOwner());
	}
	else if (event == GameEvent::EnemyKilled)
	{
		m_Score += 250;
		m_ScoreChangedEvent->NotifyObservers(GameEvent::ScoreChanged, GetOwner());
	}
	else if (event == GameEvent::Pickup)
	{
		m_Score += 50;
		m_ScoreChangedEvent->NotifyObservers(GameEvent::ScoreChanged, GetOwner());
	}
}
