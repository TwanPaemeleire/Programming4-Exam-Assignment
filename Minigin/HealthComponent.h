#pragma once
#include "Component.h"
#include <memory>

namespace Twengine 
{ 
	class Event; 
}

class HealthComponent final: public Twengine::Component
{
public:
	HealthComponent(Twengine::GameObject* owner);
	virtual ~HealthComponent() = default;
	HealthComponent(const HealthComponent& other) = delete;
	HealthComponent(HealthComponent&& other) = delete;
	HealthComponent& operator=(const HealthComponent& other) = delete;
	HealthComponent& operator=(HealthComponent&& other) = delete;

	void SetMaxHealth(int maxHealth) { m_MaxHealth = maxHealth; m_CurrentHealth = maxHealth; };
	void SetMaxLives(int maxLives) { m_MaxLives = maxLives; m_Lives = maxLives; };
	int GetLives() const { return m_Lives; }

	void TakeDamage(int amount);
	void Kill();

	Twengine::Event* GetObjectDiedEvent() const { return m_ObjectDiedEvent.get(); }

private:
	int m_MaxHealth{};
	int m_CurrentHealth{};
	int m_MaxLives{};
	int m_Lives{};

	std::unique_ptr<Twengine::Event> m_ObjectDiedEvent;
};

