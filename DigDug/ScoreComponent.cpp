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
	m_ScoreChangedEvent->NotifyObservers(GameEvent(make_sdbm_hash("ScoreChanged")), GetOwner());
}

void ScoreComponent::Notify(const GameEvent& event, Twengine::GameObject*)
{
	if (event.id == make_sdbm_hash("PlayerDied"))
	{
		m_Score -= 200;
		m_ScoreChangedEvent->NotifyObservers(GameEvent(make_sdbm_hash("ScoreChanged")), GetOwner());
	}
	else if (event.id == make_sdbm_hash("EnemyKilled"))
	{
		m_Score += 250;
		m_ScoreChangedEvent->NotifyObservers(GameEvent(make_sdbm_hash("ScoreChanged")), GetOwner());
	}
	else if (event.id == make_sdbm_hash("Pickup"))
	{
		m_Score += 50;
		m_ScoreChangedEvent->NotifyObservers(GameEvent(make_sdbm_hash("ScoreChanged")), GetOwner());
	}
}
