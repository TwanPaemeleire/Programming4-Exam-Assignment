#include "RectColliderComponent.h"
#include "DigDugPumpComponent.h"
#include "AnimationComponent.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "Renderer.h"
#include "MyTime.h"
#include "Event.h"
#include <iostream>

// WIP
DigDugPumpComponent::DigDugPumpComponent(Twengine::GameObject* owner)
	:Component(owner)
{
	m_OnPumpRetractedEvent = std::make_unique<Twengine::Event>();
	m_OnPumpEvent = std::make_unique<Twengine::Event>();
	GetOwner()->SetTag(make_sdbm_hash("DigDugPump"));
}

void DigDugPumpComponent::Start()
{
	m_PumpTexture = Twengine::ResourceManager::GetInstance().LoadTexture("DigDug/Pump.png");
	m_TextureSize = glm::vec2(m_PumpTexture->GetSize());
	m_RectColliderComponent = GetOwner()->AddComponent<Twengine::RectColliderComponent>();
	m_RectColliderComponent->SetHitBox(m_Transform->GetWorldPosition(), 0.f, 0.f);
	m_RectColliderComponent->GetOnCollisionEvent()->AddObserver(this);
}

void DigDugPumpComponent::Update()
{
	if (!m_IsStuckInEnemy)
	{
		float speed = (m_IsReturning) ? -m_PumpShootSpeed : m_PumpShootSpeed;
		m_ExposedAmount += speed * Twengine::Time::GetInstance().deltaTime;
		m_RectColliderComponent->SetHitBox(m_Transform->GetWorldPosition(), m_ExposedAmount, m_TextureSize.y);
		if (m_ExposedAmount >= m_TextureSize.x)
		{
			if (!m_IsReturning)
			{
				m_ExposedAmount = m_TextureSize.x;
				m_IsReturning = true;
			}
		}
		else if (m_ExposedAmount <= 0.f)
		{
			m_OnPumpRetractedEvent->NotifyObservers(GameEvent(make_sdbm_hash("OnPumpRetracted")), GetOwner());
			GetOwner()->MarkForDestruction();
		}
	}
	else if(!m_PumpButtonIsPressed)
	{
		m_IsReturning = true;
		m_IsStuckInEnemy = false;
	}
	else
	{
		m_PumpDelayCounter += Twengine::Time::GetInstance().deltaTime;
		if (m_PumpDelayCounter >= m_PumpDelay)
		{
			m_PumpDelayCounter -= m_PumpDelay;
			m_OnPumpEvent->NotifyObservers(GameEvent(make_sdbm_hash("OnPump")), GetOwner());
		}
	}
}

void DigDugPumpComponent::Render() const
{
	auto& pos = m_Transform->GetWorldPosition();
	float srcX = m_TextureSize.x - m_ExposedAmount;
	Twengine::Renderer::GetInstance().RenderTextureRect(*m_PumpTexture, pos.x, pos.y, srcX, 0.f, m_ExposedAmount, m_TextureSize.y);
}

void DigDugPumpComponent::OnPumpButtonInteraction(bool isPressBound)
{
	m_PumpButtonIsPressed = isPressBound;
	if (!m_PumpButtonIsPressed && m_IsStuckInEnemy)
	{
		Retract();
	}
}

void DigDugPumpComponent::Retract()
{
	m_IsReturning = true;
	m_IsStuckInEnemy = false;
}

void DigDugPumpComponent::Notify(const GameEvent& event, Twengine::GameObject* observedObject)
{
	if (event.id == make_sdbm_hash("OnCollision") && observedObject->GetTag() == make_sdbm_hash("Enemy"))
	{
		if (!m_IsReturning)
		{
			m_IsStuckInEnemy = true;
			m_IsReturning = false;
		}
	}
}