#include "PookaComponent.h"
#include "Renderer.h"
#include "GameObject.h"
#include "GameManager.h"
#include "GroundComponent.h"
#include "MyTime.h"
#include "EnemyMovementComponent.h"

PookaComponent::PookaComponent(Twengine::GameObject* owner)
	: Component(owner)
{
	m_MovementComponent = GetOwner()->AddComponent<EnemyMovementComponent>();
	m_MovementComponent->GetCanReachPlayerEvent()->AddObserver(this);
}

void PookaComponent::Start()
{
	m_GroundComponent = GameManager::GetInstance().GetGround();
}

void PookaComponent::Update()
{
	if (!m_CanMoveToPlayer)
	{
		m_MovementComponent->MovementIfNoPathToPlayer();
		return;
	}

	// Pathfinding To Player
	const glm::vec2& currentPosition = m_Transform->GetWorldPosition();
	const glm::vec2 direction = m_NextNodeToPlayer - currentPosition;
	const float distance = glm::length(direction);
	
	if (distance < 0.5f) // Next Node Reached
	{
		m_Transform->SetLocalPosition(m_NextNodeToPlayer);
		m_NextNodeToPlayer = m_GroundComponent->GetCellTargetToGetCloserToPlayer(m_Transform->GetWorldPosition());
	}
	else // Continue Moving Towards Next Node
	{
		glm::vec2 normalizedDir = glm::normalize(direction);
		glm::vec2 movement = normalizedDir * m_MovementSpeed * Twengine::Time::GetInstance().deltaTime;
		m_Transform->SetLocalPosition(currentPosition + movement);
	}	
}

void PookaComponent::RenderUI()
{
	Twengine::Renderer::GetInstance().DrawRectangle(m_NextNodeToPlayer.x, m_NextNodeToPlayer.y, 5.f, 5.f, SDL_Color(0, 255, 0, 255));
}

void PookaComponent::Notify(const GameEvent& event, Twengine::GameObject* observedObject)
{
	if (event.id == make_sdbm_hash("OnCanReachPlayer"))
	{
		observedObject;
		m_CanMoveToPlayer = true;
		m_NextNodeToPlayer = m_GroundComponent->GetCellTargetToGetCloserToPlayer(m_Transform->GetWorldPosition());
	}
}
