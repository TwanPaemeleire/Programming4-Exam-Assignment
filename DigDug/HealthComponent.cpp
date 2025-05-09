#include "HealthComponent.h"
#include "Event.h"

HealthComponent::HealthComponent(Twengine::GameObject* owner)
	:Component(owner)
{
	m_ObjectDiedEvent = std::make_unique<Twengine::Event>();
}

void HealthComponent::TakeDamage(int amount)
{
	if (m_Lives <= 0) return; // Object is out of lives
	m_CurrentHealth -= amount;
	if (m_CurrentHealth <= 0) // Object has died
	{
		m_CurrentHealth = 0;
		--m_Lives;
		m_ObjectDiedEvent->NotifyObservers(GameEvent(make_sdbm_hash("PlayerDied")), GetOwner());
	}
}

void HealthComponent::Kill()
{
	TakeDamage(m_MaxHealth);
}
