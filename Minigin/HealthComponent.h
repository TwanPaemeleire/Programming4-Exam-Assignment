#pragma once
#include "Component.h"
#include <memory>
class Subject;

class HealthComponent: public Component
{
public:
	HealthComponent(GameObject* owner);
	virtual ~HealthComponent() = default;
	HealthComponent(const HealthComponent& other) = delete;
	HealthComponent(HealthComponent&& other) = delete;
	HealthComponent& operator=(const HealthComponent& other) = delete;
	HealthComponent& operator=(HealthComponent&& other) = delete;

	void SetMaxHealth(int maxHealth) { m_MaxHealth = maxHealth; m_CurrentHealth = maxHealth; };
	void SetMaxLives(int maxLives) { m_MaxLives = maxLives; m_Lives = maxLives; };

	void TakeDamage(int amount);
	bool IsAlive() const { return m_IsAlive; }
	void Kill();

private:
	int m_MaxHealth{};
	int m_CurrentHealth{};
	int m_MaxLives{};
	int m_Lives{};
	bool m_IsAlive{true};

	std::unique_ptr<Subject> m_ObjectDiedEvent;
};

