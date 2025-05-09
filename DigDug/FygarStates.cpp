#include "FygarStates.h"
#include "AnimationComponent.h"
#include "EnemyMovementComponent.h"
#include "GroundComponent.h"
#include "GameObject.h"
#include "GameManager.h"
#include <iostream>

void FygarIdleState::OnEnter(Twengine::GameObject* stateOwner)
{
	m_MovementComp = stateOwner->GetComponent<EnemyMovementComponent>();
	m_MovementComp->SetNewIdleTarget();
	Twengine::AnimationComponent* animationComp = stateOwner->GetComponent<Twengine::AnimationComponent>();
	if (!animationComp->IsPlayingAnimation(make_sdbm_hash("FygarMove")))
	{
		animationComp->PlayAnimation(make_sdbm_hash("FygarMove"));
	}
}

std::unique_ptr<FygarState> FygarIdleState::Update(Twengine::GameObject*)
{
	if (m_MovementComp->MovementIfNoPathToPlayer()) // Fygar can now reach player
	{
		return std::make_unique<FygarTrackingState>();
	}
	if (m_MovementComp->GhostCoolDownHasFinished()) // Fygar can enter ghost form
	{
		return std::make_unique<FygarGhostState>();
	}
	return nullptr;
}

void FygarTrackingState::OnEnter(Twengine::GameObject* stateOwner)
{
	m_MovementComp = stateOwner->GetComponent<EnemyMovementComponent>();
	Twengine::AnimationComponent* animationComp = stateOwner->GetComponent<Twengine::AnimationComponent>();
	if (!animationComp->IsPlayingAnimation(make_sdbm_hash("FygarMove")))
	{
		animationComp->PlayAnimation(make_sdbm_hash("FygarMove"));
	}
}

std::unique_ptr<FygarState> FygarTrackingState::Update(Twengine::GameObject*)
{
	m_MovementComp->PathFindingToPlayer();
	if (m_MovementComp->GhostCoolDownHasFinished()) // Fygar can enter ghost form
	{
		return std::make_unique<FygarGhostState>();
	}
	return nullptr;
}

void FygarGhostState::OnEnter(Twengine::GameObject* stateOwner)
{
	m_MovementComp = stateOwner->GetComponent<EnemyMovementComponent>();
	Twengine::AnimationComponent* animationComp = stateOwner->GetComponent<Twengine::AnimationComponent>();
	animationComp->PlayAnimation(make_sdbm_hash("FygarGhost"));
}

void FygarGhostState::OnExit(Twengine::GameObject*)
{
	m_MovementComp->ResetGhostStateValues();
}

std::unique_ptr<FygarState> FygarGhostState::Update(Twengine::GameObject* stateOwner)
{
	if (m_MovementComp->MovementInGhostForm()) // Has found place to go out of ghost form
	{
		if (GameManager::GetInstance().GetGround()->EnemyCanReachPlayer(stateOwner->GetTransform()->GetWorldPosition()))
		{
			return std::make_unique<FygarTrackingState>();
		}
		return std::make_unique<FygarIdleState>();
	}
	return nullptr;
}
