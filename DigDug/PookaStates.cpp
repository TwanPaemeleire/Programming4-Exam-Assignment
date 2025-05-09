#include "PookaStates.h"
#include "AnimationComponent.h"
#include "EnemyMovementComponent.h"
#include "GameObject.h"
#include <iostream>

// IDLE
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
	if (m_MovementComp->MovementIfNoPathToPlayer()) // Pooka Can Now Reach The Player
	{
		return std::make_unique<PookaTrackingState>();
	}
	if (m_MovementComp->GhostCoolDownHasFinished()) // Pooka Can Enter Ghost Form
	{
		std::cout << "going to ghostState" << std::endl;
		return std::make_unique<PookaGhostState>();
	}
	return nullptr;
}

// TRACKING
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
	if (m_MovementComp->GhostCoolDownHasFinished()) // Pooka Can Enter Ghost Form
	{
		std::cout << "going to ghostState" << std::endl;
		return std::make_unique<PookaGhostState>();
	}
	return nullptr;
}

// GHOST
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

std::unique_ptr<PookaState> PookaGhostState::Update(Twengine::GameObject*)
{
	if (m_MovementComp->MovementInGhostForm()) // Has Found Place To Go Out Of Ghost Form
	{
		return std::make_unique<PookaIdleState>();
	}
	return nullptr;
}
