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
	m_MovementComponent->GetOnCanReachPlayerEvent()->AddObserver(this);
}

void FygarComponent::Update()
{
	if (!m_CanMoveToPlayer)
	{
		m_MovementComponent->MovementIfNoPathToPlayer();
		return;
	}

	m_MovementComponent->PathFindingToPlayer();
}

void FygarComponent::Notify(const GameEvent& event, Twengine::GameObject*)
{
	if (event.id == make_sdbm_hash("OnCanReachPlayer"))
	{
		m_CanMoveToPlayer = true;
	}
}
