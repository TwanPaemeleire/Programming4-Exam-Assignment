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
	Twengine::AnimationComponent* animComp = GetOwner()->GetComponent<Twengine::AnimationComponent>();
	animComp->AddAnimation("Pooka/PookaMove.png", make_sdbm_hash("PookaMove"), 2);
	animComp->AddAnimation("Pooka/PookaMove.png", make_sdbm_hash("PookaPump"), 4);
	animComp->AddAnimation("Pooka/PookaMove.png", make_sdbm_hash("PookaGhost"), 2);
	animComp->AddAnimation("Pooka/PookaMove.png", make_sdbm_hash("PookaCrushed"), 2);
	animComp->PlayAnimation(make_sdbm_hash("PookaMove"));
	GetOwner()->GetComponent<Twengine::RectColliderComponent>()->SetHitBox(m_Transform->GetWorldPosition(), animComp->GetAnimationFrameWidth(), animComp->GetAnimationFrameHeight());
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
