#include "FygarStates.h"
#include "AnimationComponent.h"
#include "EnemyMovementComponent.h"
#include "GroundComponent.h"
#include "FygarFireComponent.h"
#include "GameObject.h"
#include "GameManager.h"
#include "SceneManager.h"
#include <iostream>
#include "GridComponent.h"
#include "MyTime.h"
#include "DigDugPumpComponent.h"
#include "FygarComponent.h"
#include "RockComponent.h"
#include "RectColliderComponent.h"

float FygarTrackingState::m_FireCooldownCounter = 5.f;

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

std::unique_ptr<FygarState> FygarIdleState::GetNotifiedByOwner(const GameEvent& event, Twengine::GameObject* observedObject, Twengine::GameObject*)
{
	DigDugPumpComponent* digDugPumpComponent = observedObject->GetComponent<DigDugPumpComponent>();
	if (event.id == make_sdbm_hash("OnCollision") && digDugPumpComponent)
	{
		return std::make_unique<FygarPumpingState>();
	}
	else if (event.id == make_sdbm_hash("OnCollisionEnter") && observedObject->GetTag() == make_sdbm_hash("Rock"))
	{
		RockComponent* rock = observedObject->GetComponent<RockComponent>();
		return std::make_unique<FygarRockDragState>(rock);
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
	m_GridComponent = GameManager::GetInstance().GetGrid();
}

std::unique_ptr<FygarState> FygarTrackingState::Update(Twengine::GameObject* stateOwner)
{
	m_FireCooldownCounter += Twengine::Time::GetInstance().deltaTime;
	m_MovementComp->PathFindingToPlayer();

	if (m_FireCooldownCounter >= m_FireCooldown)
	{
		glm::vec2 playerPos = GameManager::GetInstance().GetPlayerTransform()->GetWorldPosition();
		glm::vec2 fygarPos = stateOwner->GetTransform()->GetWorldPosition();
		int playerRow = m_GridComponent->GetIndexFromPosition(playerPos).first;
		int fygarRow = m_GridComponent->GetIndexFromPosition(fygarPos).first;

		if (playerRow == fygarRow)
		{
			// Check if close enough to breathe fire
			if (abs(playerPos.x - fygarPos.x) <= m_DistanceToTriggerFire)
			{
				m_FireCooldownCounter = 0.f;
				return std::make_unique<FygarFireBreathingState>();
			}
		}
	}	

	if (m_MovementComp->GhostCoolDownHasFinished()) // Fygar can enter ghost form
	{
		return std::make_unique<FygarGhostState>();
	}
	return nullptr;
}

std::unique_ptr<FygarState> FygarTrackingState::GetNotifiedByOwner(const GameEvent& event, Twengine::GameObject* observedObject, Twengine::GameObject*)
{
	DigDugPumpComponent* digDugPumpComponent = observedObject->GetComponent<DigDugPumpComponent>();
	if (event.id == make_sdbm_hash("OnCollision") && digDugPumpComponent)
	{
		return std::make_unique<FygarPumpingState>();
	}
	else if (event.id == make_sdbm_hash("OnCollisionEnter") && observedObject->GetTag() == make_sdbm_hash("Rock"))
	{
		RockComponent* rock = observedObject->GetComponent<RockComponent>();
		return std::make_unique<FygarRockDragState>(rock);
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

std::unique_ptr<FygarState> FygarGhostState::GetNotifiedByOwner(const GameEvent& event, Twengine::GameObject* observedObject, Twengine::GameObject*)
{
	DigDugPumpComponent* digDugPumpComponent = observedObject->GetComponent<DigDugPumpComponent>();
	if (event.id == make_sdbm_hash("OnCollision") && digDugPumpComponent)
	{
		return std::make_unique<FygarPumpingState>();
	}
	else if (event.id == make_sdbm_hash("OnCollisionEnter") && observedObject->GetTag() == make_sdbm_hash("Rock"))
	{
		RockComponent* rock = observedObject->GetComponent<RockComponent>();
		return std::make_unique<FygarRockDragState>(rock);
	}
	return nullptr;
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

void FygarFireBreathingState::OnEnter(Twengine::GameObject* stateOwner)
{
	std::unique_ptr<Twengine::GameObject> fire = std::make_unique<Twengine::GameObject>();
	FygarFireComponent* fireComponent = fire->AddComponent<FygarFireComponent>();
	m_FireGameObject = fire.get();
	fire->SetParent(stateOwner, false);

	glm::vec2 playerPos = GameManager::GetInstance().GetPlayerTransform()->GetWorldPosition();
	glm::vec2 fygarPos = stateOwner->GetTransform()->GetWorldPosition();
	int playerColumn = GameManager::GetInstance().GetGrid()->GetIndexFromPosition(playerPos).second;
	int fygarColumn = GameManager::GetInstance().GetGrid()->GetIndexFromPosition(fygarPos).second;

	if (playerColumn >= fygarColumn) // Should shoot the fire to the right
	{
		fireComponent->ShotToRight();
	}

	//m_FireGameObject->Start(); // TEMP, REMOVE LATER, THIS IS CURRENTLY BEING ADDED IN GAME "START" AND THEREFORE NOT ACTUALLY CALLING START ON THIS OBJECT
	Twengine::SceneManager::GetInstance().GetCurrentScene().Add(std::move(fire));
}

std::unique_ptr<FygarState> FygarFireBreathingState::Update(Twengine::GameObject*)
{
	return nullptr;
}

std::unique_ptr<FygarState> FygarFireBreathingState::LateUpdate(Twengine::GameObject*)
{
	if (m_FireGameObject->IsMarkedForDestruction()) 
	{
		return std::make_unique<FygarTrackingState>();
	}
	return nullptr;
}

std::unique_ptr<FygarState> FygarFireBreathingState::GetNotifiedByOwner(const GameEvent& event, Twengine::GameObject* observedObject, Twengine::GameObject*)
{
	DigDugPumpComponent* digDugPumpComponent = observedObject->GetComponent<DigDugPumpComponent>();
	if (event.id == make_sdbm_hash("OnCollision") && digDugPumpComponent)
	{
		return std::make_unique<FygarPumpingState>();
	}
	else if (event.id == make_sdbm_hash("OnCollisionEnter") && observedObject->GetTag() == make_sdbm_hash("Rock"))
	{
		RockComponent* rock = observedObject->GetComponent<RockComponent>();
		return std::make_unique<FygarRockDragState>(rock);
	}
	return nullptr;
}

void FygarPumpingState::OnEnter(Twengine::GameObject* stateOwner)
{
	m_AnimationComponent = stateOwner->GetComponent<Twengine::AnimationComponent>();
	m_AnimationComponent->PlayAnimation(make_sdbm_hash("FygarPump"), 0.f, false);
}

std::unique_ptr<FygarState> FygarPumpingState::Update(Twengine::GameObject*)
{
	if (!m_IsBeingPumped)
	{
		m_DeflateDelayCounter += Twengine::Time::GetInstance().deltaTime;
		if (m_DeflateDelayCounter >= m_DeflateDelay)
		{
			if (m_AnimationComponent->GetCurrentFrameIndex() == 0)
			{
				return std::make_unique<FygarTrackingState>();
			}
			m_AnimationComponent->GoToPreviousFrame();
			m_DeflateDelayCounter -= m_DeflateDelay;
		}
	}
	return nullptr;
}

std::unique_ptr<FygarState> FygarPumpingState::GetNotifiedByOwner(const GameEvent& event, Twengine::GameObject* observedObject, Twengine::GameObject* stateOwner)
{
	if (event.id == make_sdbm_hash("OnPump"))
	{
		m_DeflateDelayCounter = 0.0f;
		m_AnimationComponent->GoToNextFrame();
		if (m_AnimationComponent->HasFinishedPlayingOnce())
		{
			observedObject->GetComponent<DigDugPumpComponent>()->GetOnPumpEvent()->RemoveObserver(stateOwner->GetComponent<FygarComponent>());
			return std::make_unique<FygarDeathState>();
		}
	}

	if (event.id == make_sdbm_hash("OnCollisionEnter") && observedObject->GetTag() == make_sdbm_hash("DigDugPump"))
	{
		DigDugPumpComponent* pumpComponent = observedObject->GetComponent<DigDugPumpComponent>();
		pumpComponent->GetOnPumpEvent()->AddObserver(stateOwner->GetComponent<FygarComponent>()); 
		m_IsBeingPumped = true;

		if (m_HasBeenPumpedOnce)
		{
			m_DeflateDelayCounter = 0.0f;
			m_AnimationComponent->GoToNextFrame();
			if (m_AnimationComponent->HasFinishedPlayingOnce())
			{
				return std::make_unique<FygarDeathState>();
			}
		}
		else m_HasBeenPumpedOnce = true;

	}
	if (event.id == make_sdbm_hash("OnCollisionExit") && observedObject->GetTag() == make_sdbm_hash("DigDugPump"))
	{
		m_IsBeingPumped = false;
	}
	else if (event.id == make_sdbm_hash("OnCollisionEnter") && observedObject->GetTag() == make_sdbm_hash("Rock"))
	{
		RockComponent* rock = observedObject->GetComponent<RockComponent>();
		return std::make_unique<FygarRockDragState>(rock);
	}
	return nullptr;
}

void FygarDeathState::OnEnter(Twengine::GameObject* stateOwner)
{
	// Send out an event to increase score & play a sound
	stateOwner->MarkForDestruction();
	stateOwner->GetComponent<FygarComponent>()->GetOnDeathEvent()->NotifyObservers(GameEvent(make_sdbm_hash("OnEnemyKilled")), stateOwner);
}

std::unique_ptr<FygarState> FygarDeathState::Update(Twengine::GameObject*)
{
	return nullptr;
}

FygarRockDragState::FygarRockDragState(RockComponent* rockComponent)
	:m_RockComponent{rockComponent}
{
	m_EnemyCrushedEvent = std::make_unique<Twengine::Event>();
	m_EnemyCrushedEvent->AddObserver(rockComponent);
}

void FygarRockDragState::OnEnter(Twengine::GameObject* stateOwner)
{
	m_AnimationComponent = stateOwner->GetComponent<Twengine::AnimationComponent>();
	m_AnimationComponent->PlayAnimation(make_sdbm_hash("FygarCrushed"), 0.f, false);
	stateOwner->SetParent(m_RockComponent->GetOwner(), true);
	m_AmountUnderRockToCheck = m_AnimationComponent->GetAnimationFrameHeight();
	stateOwner->GetComponent<Twengine::RectColliderComponent>()->SetEnabled(false);
}

std::unique_ptr<FygarState> FygarRockDragState::Update(Twengine::GameObject* stateOwner)
{
	glm::vec2 posToCheck = m_RockComponent->GetBottomMiddle();
	posToCheck.y += m_AmountUnderRockToCheck;
	if (!GameManager::GetInstance().GetGround()->PositionIsDugOut(posToCheck))
	{
		m_EnemyCrushedEvent->NotifyObservers(GameEvent(make_sdbm_hash("OnEnemyCrushed")), stateOwner);
	}
	return nullptr;
}
