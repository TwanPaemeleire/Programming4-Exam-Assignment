#include "PookaComponent.h"
#include "GameObject.h"
#include "AnimationComponent.h"
#include "RectColliderComponent.h"

PookaComponent::PookaComponent(Twengine::GameObject* owner)
	: Component(owner)
{
}

void PookaComponent::Start()
{
	Twengine::AnimationComponent* animationComp = GetOwner()->GetComponent<Twengine::AnimationComponent>();
	animationComp->AddAnimation("Pooka/PookaMove.png", make_sdbm_hash("PookaMove"), 2);
	animationComp->AddAnimation("Pooka/PookaPump.png", make_sdbm_hash("PookaPump"), 4);
	animationComp->AddAnimation("Pooka/PookaGhost.png", make_sdbm_hash("PookaGhost"), 2);
	animationComp->AddAnimation("Pooka/PookaCrushed.png", make_sdbm_hash("PookaCrushed"), 2);
	animationComp->PlayAnimation(make_sdbm_hash("PookaMove")); // To make sure we have a valid width and height for the hitbox
	GetOwner()->GetComponent<Twengine::RectColliderComponent>()->SetHitBox(m_Transform->GetWorldPosition(), animationComp->GetAnimationFrameWidth(), animationComp->GetAnimationFrameHeight());
	m_CurrentState = std::make_unique<PookaIdleState>();
	m_CurrentState->OnEnter(GetOwner());
}

void PookaComponent::Update()
{
	CheckAndTransitionStates(m_CurrentState->Update(GetOwner()));
}

void PookaComponent::CheckAndTransitionStates(std::unique_ptr<PookaState> newState)
{
	if (newState && typeid(*m_CurrentState) != typeid(*newState))
	{
		m_CurrentState->OnExit(GetOwner());
		m_CurrentState = std::move(newState);
		m_CurrentState->OnEnter(GetOwner());
	}
}
