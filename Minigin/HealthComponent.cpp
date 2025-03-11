#include "HealthComponent.h"
#include "Subject.h"

HealthComponent::HealthComponent(GameObject* owner)
	:Component(owner)
{
	m_ObjectDiedEvent = std::make_unique<Subject>();
}

void HealthComponent::TakeDamage(int amount)
{
	if (m_Lives <= 0) return; // Object Is Out Of Lives
	m_CurrentHealth -= amount;
	if (m_CurrentHealth <= 0) // Object Has Died
	{
		m_CurrentHealth = 0;
		--m_Lives;
		m_ObjectDiedEvent->NotifyObservers(GameEvent::PlayerDied, GetOwner());
	}
}

void HealthComponent::Kill()
{
	TakeDamage(m_MaxHealth);
}
