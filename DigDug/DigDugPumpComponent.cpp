#include "RectColliderComponent.h"
#include "DigDugPumpComponent.h"
#include "AnimationComponent.h"
#include "GroundComponent.h"
#include "ResourceManager.h"
#include "GameManager.h"
#include "GameObject.h"
#include "Renderer.h"
#include "MyTime.h"
#include <iostream>
#include "Event.h"

DigDugPumpComponent::DigDugPumpComponent(Twengine::GameObject* owner)
	:Component(owner)
{
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
}

void DigDugPumpComponent::Update()
{
	if (!m_IsStuckInEnemy)
	{
		float speed = (m_IsReturning) ? -m_PumpShootSpeed : m_PumpShootSpeed;
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
	float sourceY{};
	float sourceWidth{};
	float sourceHeight{};
	if (m_ShotVertically)
	{
		sourceX = 0.f;
		sourceY = 0.f;
		sourceWidth = m_TextureSize.x;
		sourceHeight = m_ExposedAmount;
	}
	else
	{
		sourceX = m_TextureSize.x - m_ExposedAmount;
		sourceY = 0.f;
		sourceWidth = m_ExposedAmount;
		sourceHeight = m_TextureSize.y;
	}
	bool verticalFlip = (m_ShotVertically && !m_HorizontallyFlipped);

	Twengine::Renderer::GetInstance().RenderTextureRect(*m_PumpTexture, m_DrawPosition.x, m_DrawPosition.y, sourceX,
		sourceY, sourceWidth, sourceHeight,
		m_HorizontallyFlipped,
		verticalFlip, 0.0);
}

void DigDugPumpComponent::RenderUI()
{
	auto& pos = m_Transform->GetWorldPosition();
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
	if (!m_ShotVertically && !m_HorizontallyFlipped) // Right
	{
		m_RectColliderComponent->SetHitBox(worldPos, m_ExposedAmount, m_TextureSize.y);
	}
	else if (!m_ShotVertically && m_HorizontallyFlipped) // Left
	{
		worldPos.x -= m_ExposedAmount;
		m_RectColliderComponent->SetHitBox(worldPos, m_ExposedAmount, m_TextureSize.y);
	}
	else if (m_ShotVertically && m_HorizontallyFlipped) // Up
	{
		worldPos.y -= m_ExposedAmount;
		m_RectColliderComponent->SetHitBox(worldPos, m_TextureSize.x, m_ExposedAmount);
	}
	else if (m_ShotVertically && !m_HorizontallyFlipped) // Down
	{
		m_RectColliderComponent->SetHitBox(worldPos, m_TextureSize.x, m_ExposedAmount);
	}
}

bool DigDugPumpComponent::NextPosIsDugOut()
{
	glm::vec2 worldPos = m_Transform->GetWorldPosition();
	if (!m_ShotVertically && !m_HorizontallyFlipped) // Right
	{
		return m_GroundComponent->PositionIsDugOut(glm::vec2(worldPos.x + m_ExposedAmount, worldPos.y));
	}
	else if (!m_ShotVertically && m_HorizontallyFlipped) // Left
	{
		return m_GroundComponent->PositionIsDugOut(glm::vec2(worldPos.x - m_ExposedAmount, worldPos.y));
	}
	else if (m_ShotVertically && m_HorizontallyFlipped) // Up
	{
		return m_GroundComponent->PositionIsDugOut(glm::vec2(worldPos.x, worldPos.y - m_ExposedAmount));
	}
	else if (m_ShotVertically && !m_HorizontallyFlipped) // Down
	{
		return m_GroundComponent->PositionIsDugOut(glm::vec2(worldPos.x, worldPos.y + m_ExposedAmount));
	}
	return false;
}

void DigDugPumpComponent::SetDrawPosition()
{
	auto worldPos = m_Transform->GetWorldPosition();

	if (!m_ShotVertically && !m_HorizontallyFlipped) // Right
	{
		m_DrawPosition = worldPos;
	}
	else if (!m_ShotVertically && m_HorizontallyFlipped) // Left
	{
		m_DrawPosition = { worldPos.x - m_ExposedAmount, worldPos.y };
	}
	else if (m_ShotVertically && m_HorizontallyFlipped) // Up
	{
		m_DrawPosition = { worldPos.x, worldPos.y - m_ExposedAmount };
	}
	else if (m_ShotVertically && !m_HorizontallyFlipped) // Down
	{
		m_DrawPosition = worldPos;
	}
}
