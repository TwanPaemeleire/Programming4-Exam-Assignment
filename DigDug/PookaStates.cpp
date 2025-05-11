#include "PookaStates.h"
#include "AnimationComponent.h"
#include "EnemyMovementComponent.h"
#include "GroundComponent.h"
#include "GameObject.h"
#include "GameManager.h"
#include <iostream>

// Idle
void PookaIdleState::OnEnter(Twengine::GameObject* stateOwner)
{
	m_MovementComp = stateOwner->GetComponent<EnemyMovementComponent>();
	m_MovementComp->SetNewIdleTarget();
	Twengine::AnimationComponent* animationComp = stateOwner->GetComponent<Twengine::AnimationComponent>();
	if (!animationComp->IsPlayingAnimation(make_sdbm_hash("PookaMove")))
	{
		animationComp->PlayAnimation(make_sdbm_hash("PookaMove"));
	}
}

std::unique_ptr<PookaState> PookaIdleState::Update(Twengine::GameObject*)
{
	if (m_MovementComp->MovementIfNoPathToPlayer()) // Pooka can now reach player
	{
		return std::make_unique<PookaTrackingState>();
	}
	if (m_MovementComp->GhostCoolDownHasFinished()) // Pooka can enter ghost form
	{
		return std::make_unique<PookaGhostState>();
	}
	return nullptr;
}

// Tracking
void PookaTrackingState::OnEnter(Twengine::GameObject* stateOwner)
{
	m_MovementComp = stateOwner->GetComponent<EnemyMovementComponent>();
	Twengine::AnimationComponent* animationComp = stateOwner->GetComponent<Twengine::AnimationComponent>();
	if (!animationComp->IsPlayingAnimation(make_sdbm_hash("PookaMove")))
	{
		animationComp->PlayAnimation(make_sdbm_hash("PookaMove"));
	}
}

std::unique_ptr<PookaState> PookaTrackingState::Update(Twengine::GameObject*)
{
	m_MovementComp->PathFindingToPlayer();
	if (m_MovementComp->GhostCoolDownHasFinished()) // Pooka can enter ghost form
	{
		return std::make_unique<PookaGhostState>();
	}
	return nullptr;
}

// Ghost
void PookaGhostState::OnEnter(Twengine::GameObject* stateOwner)
{
	m_MovementComp = stateOwner->GetComponent<EnemyMovementComponent>();
	Twengine::AnimationComponent* animationComp = stateOwner->GetComponent<Twengine::AnimationComponent>();
	animationComp->PlayAnimation(make_sdbm_hash("PookaGhost"));
}

void PookaGhostState::OnExit(Twengine::GameObject*)
{
	m_MovementComp->ResetGhostStateValues();
}

std::unique_ptr<PookaState> PookaGhostState::Update(Twengine::GameObject* stateOwner)
{
	if (m_MovementComp->MovementInGhostForm()) // Has found place to go out of ghost form
	{
		if (GameManager::GetInstance().GetGround()->EnemyCanReachPlayer(stateOwner->GetTransform()->GetWorldPosition()))
		{
			return std::make_unique<PookaTrackingState>();
		}
		return std::make_unique<PookaIdleState>();
	}
	return nullptr;
}

// Pumping (WIP)
void PookaPumpingState::OnEnter(Twengine::GameObject* stateOwner)
{
	m_AnimationComponent = stateOwner->GetComponent<Twengine::AnimationComponent>();
	m_AnimationComponent->PlayAnimation(make_sdbm_hash("PookaPump"), 0.f, false);
}

std::unique_ptr<PookaState> PookaPumpingState::Update(Twengine::GameObject*)
{
	return nullptr;
}

// Death (WIP)
void PookaDeathState::OnEnter(Twengine::GameObject*)
{
	// Send out an event to increase score
}

std::unique_ptr<PookaState> PookaDeathState::Update(Twengine::GameObject*)
{
	return nullptr;
}