#include "PointComponent.h"
#include "Event.h"

PointComponent::PointComponent(GameObject* owner)
	:Component(owner)
{
	m_ScoreChangedEvent = std::make_unique<Event>();
}

void PointComponent::ChangePoints(int amount)
{
	m_Points += amount;
	m_ScoreChangedEvent->NotifyObservers(GameEvent::PointsChanged, GetOwner());
}

void PointComponent::SetPoints(int points)
{
	m_Points = points;
	m_ScoreChangedEvent->NotifyObservers(GameEvent::PointsChanged, GetOwner());
}
