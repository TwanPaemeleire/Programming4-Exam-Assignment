#include "RectColliderComponent.h"
#include "DigDugPumpComponent.h"
#include "AnimationComponent.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "Renderer.h"
#include "MyTime.h"
#include "Event.h"

// WIP
DigDugPumpComponent::DigDugPumpComponent(Twengine::GameObject* owner)
	:Component(owner)
{
	m_OnPumpRetractedEvent = std::make_unique<Twengine::Event>();
}

void DigDugPumpComponent::Start()
{
	m_PumpTexture = Twengine::ResourceManager::GetInstance().LoadTexture("DigDug/Pump.png");
	m_TextureSize = glm::vec2(m_PumpTexture->GetSize());
	m_RectColliderComponent = GetOwner()->AddComponent<Twengine::RectColliderComponent>();
	m_RectColliderComponent->SetHitBox(m_Transform->GetWorldPosition(), m_TextureSize.x, m_TextureSize.y);
}

void DigDugPumpComponent::Update()
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

void DigDugPumpComponent::Render() const
{
	auto& pos = m_Transform->GetWorldPosition();
	float srcX = m_TextureSize.x - m_ExposedAmount;
	Twengine::Renderer::GetInstance().RenderTextureRect(*m_PumpTexture, pos.x, pos.y, srcX, 0.f, m_ExposedAmount, m_TextureSize.y);
}

void DigDugPumpComponent::Notify(const GameEvent& event, Twengine::GameObject* observedObject)
{
	if (event.id == make_sdbm_hash("OnCollision") && observedObject->GetTag() == make_sdbm_hash("Enemy"))
	{
		m_IsReturning = true;
	}
}