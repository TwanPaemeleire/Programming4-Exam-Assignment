#include "FygarComponent.h"
#include "GameObject.h"
#include "EnemyMovementComponent.h"
#include "RectColliderComponent.h"
#include "AnimationComponent.h"

FygarComponent::FygarComponent(Twengine::GameObject* owner)
	:Component(owner)
{
	m_MovementComponent = GetOwner()->AddComponent<EnemyMovementComponent>();
	m_MovementComponent->GetOnCanReachPlayerEvent()->AddObserver(this);
}

void FygarComponent::Start()
{
	Twengine::AnimationComponent* animComp = GetOwner()->GetComponent<Twengine::AnimationComponent>();
	GetOwner()->GetComponent<Twengine::RectColliderComponent>()->SetHitBox(m_Transform->GetWorldPosition(), animComp->GetAnimationFrameWidth(), animComp->GetAnimationFrameHeight());
}

void FygarComponent::Update()
{
	if (!m_CanMoveToPlayer)
	{
		m_MovementComponent->MovementIfNoPathToPlayer();
		return;
	}

	m_MovementComponent->PathFindingToPlayer();
	// In The Future, Add Code Here For Distance & Reachability Check To Player To Determine When To Trigger Attack
}

void FygarComponent::Notify(const GameEvent& event, Twengine::GameObject*)
{
	if (event.id == make_sdbm_hash("OnCanReachPlayer"))
	{
		m_CanMoveToPlayer = true;
	}
}
