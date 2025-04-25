#include "PookaComponent.h"
#include "GameObject.h"
#include "EnemyMovementComponent.h"
#include "AnimationComponent.h"
#include "RectColliderComponent.h"

PookaComponent::PookaComponent(Twengine::GameObject* owner)
	: Component(owner)
{
	m_MovementComponent = GetOwner()->AddComponent<EnemyMovementComponent>();
	m_MovementComponent->GetOnCanReachPlayerEvent()->AddObserver(this);
}

void PookaComponent::Start()
{
	Twengine::AnimationComponent* animComp = GetOwner()->GetComponent<Twengine::AnimationComponent>();
	GetOwner()->GetComponent<Twengine::RectColliderComponent>()->SetHitBox(m_Transform->GetWorldPosition(), animComp->GetAnimationFrameWidth(), animComp->GetAnimationFrameHeight());
}

void PookaComponent::Update()
{
	if (!m_CanMoveToPlayer)
	{
		m_MovementComponent->MovementIfNoPathToPlayer();
		return;
	}
	
	m_MovementComponent->PathFindingToPlayer();
}

void PookaComponent::Notify(const GameEvent& event, Twengine::GameObject*)
{
	if (event.id == make_sdbm_hash("OnCanReachPlayer"))
	{
		m_CanMoveToPlayer = true;
	}
}
