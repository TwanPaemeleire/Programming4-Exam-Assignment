#include "FygarComponent.h"
#include "Renderer.h"
#include "GameObject.h"
#include "GameManager.h"
#include "GroundComponent.h"
#include "MyTime.h"
#include "EnemyMovementComponent.h"

FygarComponent::FygarComponent(Twengine::GameObject* owner)
	:Component(owner)
{
	m_MovementComponent = GetOwner()->AddComponent<EnemyMovementComponent>();
	m_MovementComponent->GetCanReachPlayerEvent()->AddObserver(this);
}

void FygarComponent::Start()
{
	m_GroundComponent = GameManager::GetInstance().GetGround();
}

void FygarComponent::Update()
{
	if (!m_CanMoveToPlayer)
	{
		m_MovementComponent->MovementIfNoPathToPlayer();
		return;
	}
}

void FygarComponent::RenderUI()
{
	Twengine::Renderer::GetInstance().DrawRectangle(m_NextNodeToPlayer.x, m_NextNodeToPlayer.y, 5.f, 5.f, SDL_Color(0, 255, 0, 255));
}

void FygarComponent::Notify(const GameEvent& event, Twengine::GameObject* observedObject)
{
	if (event.id == make_sdbm_hash("OnCanReachPlayer"))
	{
		observedObject;
		m_CanMoveToPlayer = true;
		m_NextNodeToPlayer = m_GroundComponent->GetCellTargetToGetCloserToPlayer(m_Transform->GetWorldPosition());
	}
}
