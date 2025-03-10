#include "HealthComponent.h"

HealthComponent::HealthComponent(GameObject* owner)
	:Component(owner)
{
}

void HealthComponent::TakeDamage(int amount)
{
	m_CurrentHealth -= amount;
	if (m_CurrentHealth <= 0) // Has Died
	{
		m_CurrentHealth = 0;
		m_IsAlive = false;
	}
}

void HealthComponent::Kill()
{
	TakeDamage(m_MaxHealth);
}
