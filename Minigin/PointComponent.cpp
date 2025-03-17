#include "PointComponent.h"
#include "Event.h"

PointComponent::PointComponent(Twengine::GameObject* owner)
	:Twengine::Component(owner)
{
	m_ScoreChangedEvent = std::make_unique<Twengine::Event>();
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
