#include "DigDugComponent.h"
#include "MyTime.h"
#include "GameObject.h"
#include "AnimationComponent.h"
#include "RectColliderComponent.h"
#include <iostream>
#include "ServiceLocator.h"
#include "SoundSystem.h"
#include "GameManager.h"
#include "GridComponent.h"
#include "EnemyMovementComponent.h"
#include "PlayerStates.h"
#include "Event.h"

DigDugComponent::DigDugComponent(Twengine::GameObject* owner)
	:Component(owner)
{
	m_PlayerMovingData = std::make_unique<PlayerMovingData>();
	m_AnimationComponent = GetOwner()->AddComponent<Twengine::AnimationComponent>();
	m_RectColliderComponent = GetOwner()->AddComponent<Twengine::RectColliderComponent>();
	m_RectColliderComponent->GetOnCollisionEvent()->AddObserver(this);
}

void DigDugComponent::Start()
{
	m_Transform->SetLocalPosition(GameManager::GetInstance().GetGrid()->GetPositionFromIndex(9, 5));

	m_AnimationComponent->AddAnimation("DigDug/DigDugMove.png", make_sdbm_hash("DigDugMove"),2);
	m_AnimationComponent->AddAnimation("DigDug/DigDugDigging.png", make_sdbm_hash("DigDugDigging"), 2);
	m_AnimationComponent->AddAnimation("DigDug/DigDugIdle.png", make_sdbm_hash("DigDugIdle"), 1);
	m_AnimationComponent->AddAnimation("DigDug/DigDugPump.png", make_sdbm_hash("DigDugPump"), 2);
	m_AnimationComponent->AddAnimation("DigDug/DigDugDeath.png", make_sdbm_hash("DigDugDeath"), 5, 4);
	m_AnimationComponent->AddAnimation("DigDug/DigDugCrushed.png", make_sdbm_hash("DigDugCrushed"), 1);
	m_AnimationComponent->PlayAnimation(make_sdbm_hash("DigDugIdle"));

	const glm::vec2 pos = GetOwner()->GetTransform()->GetWorldPosition();
	m_RectColliderComponent->ChangeHitBox(pos, m_AnimationComponent->GetAnimationFrameWidth(), m_AnimationComponent->GetAnimationFrameHeight());
	Twengine::ServiceLocator::get_sound_system().RequestLoadSound("DigDug/DigDugDeath.wav", make_sdbm_hash("DigDugDeath"));
	m_CurrentState = std::make_unique<PlayerMoving>();
	m_CurrentState->OnEnter(GetOwner());
}

void DigDugComponent::Update()
{
	CheckAndTransitionStates(m_CurrentState->Update(GetOwner()));
}

void DigDugComponent::RenderUI()
{
	m_CurrentState->RenderDebugDrawing();
}

void DigDugComponent::SetDirection(glm::vec2 dir)
{
	CheckAndTransitionStates(m_CurrentState->SetXDirection(GetOwner(), dir.x, dir.y));
	CheckAndTransitionStates(m_CurrentState->SetYDirection(GetOwner(), dir.y, dir.x));
}

void DigDugComponent::OnPumpButtonInteraction(Twengine::InteractionStates interactionState)
{
	CheckAndTransitionStates(m_CurrentState->OnPumpButtonInteraction(GetOwner(), interactionState));
}

void DigDugComponent::Notify(const GameEvent& event, Twengine::GameObject* observedObject)
{
	CheckAndTransitionStates(m_CurrentState->Notify(observedObject, event));
}

void DigDugComponent::Reset()
{
	m_Transform->SetLocalPosition(GameManager::GetInstance().GetGrid()->GetPositionFromIndex(9, 5));
	const glm::vec2 pos = GetOwner()->GetTransform()->GetWorldPosition();
	m_AnimationComponent->PlayAnimation(make_sdbm_hash("DigDugIdle"));
	m_RectColliderComponent->ChangeHitBox(pos, m_AnimationComponent->GetAnimationFrameWidth(), m_AnimationComponent->GetAnimationFrameHeight());
	
	m_PlayerMovingData->direction = {0.f, 0.f};
	m_PlayerMovingData->lastNonNullDirection = {};
	m_PlayerMovingData->targetPosition = { -1.f, -1.f };
	m_PlayerMovingData->currentInputDirection = { 0.f, 0.f };
	m_PlayerMovingData->distanceToTarget = {};
	m_PlayerMovingData->distanceTracker = {};
	m_PlayerMovingData->currentIndex = {};
	m_PlayerMovingData->isMoving = false;

	m_CurrentState = std::make_unique<PlayerMoving>();
	m_CurrentState->OnEnter(GetOwner());
}

void DigDugComponent::CheckAndTransitionStates(std::unique_ptr<PlayerState> newState)
{
	if (newState && typeid(*m_CurrentState) != typeid(*newState))
	{
		m_CurrentState->OnExit(GetOwner());
		m_CurrentState = std::move(newState);
		m_CurrentState->OnEnter(GetOwner());
	}
}
