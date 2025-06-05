#include "LivesComponent.h"
#include "Event.h"
#include "DigDugComponent.h"
#include "GameObject.h"
#include "GameManager.h"

LivesComponent::LivesComponent(Twengine::GameObject* owner)
	:Component(owner)
{
	m_ObjectDiedEvent = std::make_unique<Twengine::Event>();
	m_ObjectDiedEvent->AddObserver(&GameManager::GetInstance());
}

void LivesComponent::Kill()
{
	if (m_Lives <= 0) return;
	--m_Lives;
	m_ObjectDiedEvent->NotifyObservers(GameEvent(make_sdbm_hash("PlayerDied")), GetOwner());
}
