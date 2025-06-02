#include "PookaStates.h"
#include "AnimationComponent.h"
#include "EnemyMovementComponent.h"
#include "GroundComponent.h"
#include "GameObject.h"
#include "GameManager.h"
#include "DigDugPumpComponent.h"
#include "PookaComponent.h"
#include <iostream>
#include "MyTime.h"

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

std::unique_ptr<PookaState> PookaIdleState::GetNotifiedByOwner(const GameEvent& event, Twengine::GameObject* observedObject, Twengine::GameObject*)
{
	DigDugPumpComponent* digDugPumpComponent = observedObject->GetComponent<DigDugPumpComponent>();
	if (event.id == make_sdbm_hash("OnCollision") && digDugPumpComponent)
	{
		return std::make_unique<PookaPumpingState>(digDugPumpComponent);
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

std::unique_ptr<PookaState> PookaTrackingState::GetNotifiedByOwner(const GameEvent& event, Twengine::GameObject* observedObject, Twengine::GameObject*)
{
	DigDugPumpComponent* digDugPumpComponent = observedObject->GetComponent<DigDugPumpComponent>();
	if (event.id == make_sdbm_hash("OnCollision") && digDugPumpComponent)
	{
		return std::make_unique<PookaPumpingState>(digDugPumpComponent);
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

std::unique_ptr<PookaState> PookaGhostState::GetNotifiedByOwner(const GameEvent& event, Twengine::GameObject* observedObject, Twengine::GameObject*)
{
	DigDugPumpComponent* digDugPumpComponent = observedObject->GetComponent<DigDugPumpComponent>();
	if (event.id == make_sdbm_hash("OnCollision") && digDugPumpComponent)
	{
		return std::make_unique<PookaPumpingState>(digDugPumpComponent);
	}
	return nullptr;
}

PookaPumpingState::PookaPumpingState(DigDugPumpComponent* digDugPumpComponent)
{
	m_DigDugPumpComponent = digDugPumpComponent;
}

// Pumping (WIP)
void PookaPumpingState::OnEnter(Twengine::GameObject* stateOwner)
{
	m_DigDugPumpComponent->GetOnPumpEvent()->AddObserver(stateOwner->GetComponent<PookaComponent>());
	m_AnimationComponent = stateOwner->GetComponent<Twengine::AnimationComponent>();
	m_AnimationComponent->PlayAnimation(make_sdbm_hash("PookaPump"), 0.f, false);
}

std::unique_ptr<PookaState> PookaPumpingState::Update(Twengine::GameObject*)
{
	if (!m_IsBeingPumped)
	{
		m_DeflateDelayCounter += Twengine::Time::GetInstance().deltaTime;
		if (m_DeflateDelayCounter >= m_DeflateDelay)
		{
			if (m_AnimationComponent->GetCurrentFrameIndex() == 0)
			{
				return std::make_unique<PookaTrackingState>();
			}
			m_AnimationComponent->GoToPreviousFrame();
			m_DeflateDelayCounter -= m_DeflateDelay;
		}
	}
	return nullptr;
}

std::unique_ptr<PookaState> PookaPumpingState::GetNotifiedByOwner(const GameEvent& event, Twengine::GameObject* observedObject, Twengine::GameObject* stateOwner)
{
	if (event.id == make_sdbm_hash("OnPump"))
	{
		m_DeflateDelayCounter = 0.0f;
		m_AnimationComponent->GoToNextFrame();
		if (m_AnimationComponent->HasFinishedPlayingOnce())
		{
			return std::make_unique<PookaDeathState>();
		}
	}

	if (event.id == make_sdbm_hash("OnCollisionEnter") && observedObject->GetTag() == make_sdbm_hash("DigDugPump"))
	{
		m_DigDugPumpComponent = observedObject->GetComponent<DigDugPumpComponent>();
		m_DigDugPumpComponent->GetOnPumpEvent()->AddObserver(stateOwner->GetComponent<PookaComponent>());
		m_IsBeingPumped = true;
	}
	if (event.id == make_sdbm_hash("OnCollisionExit") && observedObject->GetTag() == make_sdbm_hash("DigDugPump"))
	{
		m_IsBeingPumped = false;
	}
	return nullptr;
}

// Death (WIP)
void PookaDeathState::OnEnter(Twengine::GameObject* stateOwner)
{
	// Send out an event to increase score & play a sound
	stateOwner->MarkForDestruction();
	stateOwner->GetComponent<PookaComponent>()->GetOnDeathEvent()->NotifyObservers(GameEvent(make_sdbm_hash("OnEnemyKilled")), stateOwner);
}

std::unique_ptr<PookaState> PookaDeathState::Update(Twengine::GameObject*)
{
	return nullptr;
}