#include "RectColliderComponent.h"
#include "DigDugPumpComponent.h"
#include "AnimationComponent.h"
#include "GroundComponent.h"
#include "ResourceManager.h"
#include "ServiceLocator.h"
#include "GameManager.h"
#include "SoundSystem.h"
#include "GameObject.h"
#include "Renderer.h"
#include "MyTime.h"
#include <iostream>
#include "Event.h"

DigDugPumpComponent::DigDugPumpComponent(Twengine::GameObject* owner)
	:Component(owner)
{
	Twengine::ServiceLocator::get_sound_system().RequestLoadSound("DigDug/DigDugShot.wav", make_sdbm_hash("DigDugShot"));
	m_OnPumpRetractedEvent = std::make_unique<Twengine::Event>();
	m_OnPumpEvent = std::make_unique<Twengine::Event>();
	GetOwner()->SetTag(make_sdbm_hash("DigDugPump"));
}

void DigDugPumpComponent::Start()
{
	if (m_ShotVertically)
	{
		m_PumpTexture = Twengine::ResourceManager::GetInstance().LoadTexture("DigDug/PumpVertical.png");
	}
	else
	{
		m_PumpTexture = Twengine::ResourceManager::GetInstance().LoadTexture("DigDug/Pump.png");
	}
	m_TextureSize = glm::vec2(m_PumpTexture->GetSize());
	m_ExposedAmountToCheck = (m_ShotVertically) ? m_TextureSize.y : m_TextureSize.x;
	m_RectColliderComponent = GetOwner()->AddComponent<Twengine::RectColliderComponent>();
	m_RectColliderComponent->SetHitBox(m_Transform->GetWorldPosition(), 0.f, 0.f);
	m_RectColliderComponent->GetOnCollisionEvent()->AddObserver(this);
	m_GroundComponent = GameManager::GetInstance().GetGround();
	Twengine::ServiceLocator::get_sound_system().RequestPlaySound(make_sdbm_hash("DigDugShot"), 0.05f);
}

void DigDugPumpComponent::Update()
{
	if (!m_IsStuckInEnemy)
	{
		const float speed = (m_IsReturning) ? -m_PumpShootSpeed : m_PumpShootSpeed;
		m_ExposedAmount += speed * Twengine::Time::GetInstance().deltaTime;
		if(!NextPosIsDugOut())
		{
			Retract();
		}
		SetHitBoxBasedOnDirection();
		if (m_ExposedAmount >= m_ExposedAmountToCheck)
		{
			if (!m_IsReturning)
			{
				m_ExposedAmount = m_ExposedAmountToCheck;
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
		Retract();
	}
	else
	{
		m_PumpDelayCounter += Twengine::Time::GetInstance().deltaTime;
		if (m_PumpDelayCounter >= m_PumpDelay)
		{
			m_PumpDelayCounter -= m_PumpDelay;
			m_OnPumpEvent->NotifyObservers(GameEvent(make_sdbm_hash("OnPump")), GetOwner());
			if (GameManager::GetInstance().AmountOfEnemiesAlive() <= 0)
			{
				Retract();
				return;
			}
			if (m_OnPumpEvent->GetObserverCount() == 0)
			{
				Retract();
			}
		}
	}
	SetDrawPosition();
}

void DigDugPumpComponent::Render() const
{
	float sourceX{};
	float sourceY{0.f};
	float sourceWidth{};
	float sourceHeight{};
	if (m_ShotVertically)
	{
		sourceX = 0.f;
		sourceWidth = m_TextureSize.x;
		sourceHeight = m_ExposedAmount;
	}
	else
	{
		sourceX = m_TextureSize.x - m_ExposedAmount;
		sourceWidth = m_ExposedAmount;
		sourceHeight = m_TextureSize.y;
	}
	const bool verticalFlip = (m_ShotVertically && !m_HorizontallyFlipped);

	Twengine::Renderer::GetInstance().RenderTextureRect(*m_PumpTexture, m_DrawPosition.x, m_DrawPosition.y, sourceX,
		sourceY, sourceWidth, sourceHeight,
		m_HorizontallyFlipped,
		verticalFlip, 0.0);
}

void DigDugPumpComponent::RenderUI()
{
	const auto& pos = m_Transform->GetWorldPosition();
	Twengine::Renderer::GetInstance().DrawPoint(pos.x, pos.y, SDL_Color(0, 0, 255), 5);
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

void DigDugPumpComponent::SetHitBoxBasedOnDirection()
{
	glm::vec2 worldPos = m_Transform->GetWorldPosition();
	if (!m_ShotVertically)
	{
		if(m_HorizontallyFlipped) worldPos.x -= m_ExposedAmount;
		m_RectColliderComponent->SetHitBox(worldPos, m_ExposedAmount, m_TextureSize.y);
	}
	else
	{
		if(m_HorizontallyFlipped) worldPos.y -= m_ExposedAmount;
		m_RectColliderComponent->SetHitBox(worldPos, m_TextureSize.x, m_ExposedAmount);
	}
}

bool DigDugPumpComponent::NextPosIsDugOut()
{
	glm::vec2 worldPos = m_Transform->GetWorldPosition();
	if (!m_ShotVertically)
	{
		worldPos.x += (m_HorizontallyFlipped) ? -m_ExposedAmount : m_ExposedAmount;
		if (m_GroundComponent->PositionIsDugOut(worldPos)) return true;
		worldPos.x += (m_HorizontallyFlipped) ? -m_DirtLeeway - 1.f : m_DirtLeeway + 1.f; // +1.f To make sure we're checking the position that could be right passed the dirt leeway
		return m_GroundComponent->CanMoveBetween(m_Transform->GetWorldPosition(), worldPos, m_DirtLeeway);
	}
	else
	{
		worldPos.y += (m_HorizontallyFlipped) ? -m_ExposedAmount : m_ExposedAmount;
		if (m_GroundComponent->PositionIsDugOut(worldPos)) return true;
		worldPos.y += (m_HorizontallyFlipped) ? -m_DirtLeeway - 1.f : m_DirtLeeway + 1.f; // +1.f To make sure we're checking the position that could be right passed the dirt leeway
		return m_GroundComponent->CanMoveBetween(m_Transform->GetWorldPosition(), worldPos, m_DirtLeeway);
	}
}

void DigDugPumpComponent::SetDrawPosition()
{
	const auto& worldPos = m_Transform->GetWorldPosition();
	m_DrawPosition = worldPos;
	if (m_HorizontallyFlipped)
	{
		if (m_ShotVertically) m_DrawPosition.y -= m_ExposedAmount;
		else m_DrawPosition.x -= m_ExposedAmount;
	}
}
