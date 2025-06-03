#include "FygarComponent.h"
#include "GameObject.h"
#include "EnemyMovementComponent.h"
#include "RectColliderComponent.h"
#include "AnimationComponent.h"

FygarComponent::FygarComponent(Twengine::GameObject* owner)
	:Component(owner)
{
	GetOwner()->SetTag(make_sdbm_hash("Enemy"));
	m_OnDeathEvent = std::make_unique<Twengine::Event>();
}

void FygarComponent::Start()
{
	Twengine::AnimationComponent* animationComp = GetOwner()->GetComponent<Twengine::AnimationComponent>();
	animationComp->AddAnimation("Fygar/FygarMove.png", make_sdbm_hash("FygarMove"), 2);
	animationComp->AddAnimation("Fygar/FygarPump.png", make_sdbm_hash("FygarPump"), 4);
	animationComp->AddAnimation("Fygar/FygarGhost.png", make_sdbm_hash("FygarGhost"), 2);
	animationComp->AddAnimation("Fygar/FygarCrushed.png", make_sdbm_hash("FygarCrushed"), 2);
	animationComp->PlayAnimation(make_sdbm_hash("FygarMove")); // To make sure we have a valid width and height for the hitbox
	Twengine::RectColliderComponent* rectColliderComponent = GetOwner()->GetComponent<Twengine::RectColliderComponent>();
	rectColliderComponent->SetHitBox(m_Transform->GetWorldPosition(), animationComp->GetAnimationFrameWidth(), animationComp->GetAnimationFrameHeight());
	rectColliderComponent->GetOnCollisionEvent()->AddObserver(this);
	m_CurrentState = std::make_unique<FygarIdleState>();
	m_CurrentState->OnEnter(GetOwner());
}

void FygarComponent::Update()
{
	CheckAndTransitionStates(m_CurrentState->Update(GetOwner()));
}

void FygarComponent::LateUpdate()
{
	CheckAndTransitionStates(m_CurrentState->LateUpdate(GetOwner()));
}

void FygarComponent::Notify(const GameEvent& event, Twengine::GameObject* observedObject)
{
	CheckAndTransitionStates(m_CurrentState->GetNotifiedByOwner(event, observedObject, GetOwner()));
}

void FygarComponent::CheckAndTransitionStates(std::unique_ptr<FygarState> newState)
{
	if (newState && typeid(*m_CurrentState) != typeid(*newState))
	{
		m_CurrentState->OnExit(GetOwner());
		m_CurrentState = std::move(newState);
		m_CurrentState->OnEnter(GetOwner());
	}
}
