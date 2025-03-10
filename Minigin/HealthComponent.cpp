#include "HealthComponent.h"
#include "Subject.h"

HealthComponent::HealthComponent(GameObject* owner)
	:Component(owner)
{
	m_ObjectDiedEvent = std::make_unique<Subject>();
}

void HealthComponent::TakeDamage(int amount)
{
	m_CurrentHealth -= amount;
	if (m_CurrentHealth <= 0) // Has Died
	{
		m_CurrentHealth = 0;
		m_IsAlive = false;
		m_ObjectDiedEvent->NotifyObservers(GameEvent::PlayerDied, GetOwner());
	}
}

void HealthComponent::Kill()
{
	TakeDamage(m_MaxHealth);
}
