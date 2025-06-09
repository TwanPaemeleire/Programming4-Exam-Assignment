#include "PookaComponent.h"
#include "GameObject.h"
#include "AnimationComponent.h"
#include "RectColliderComponent.h"
#include "ServiceLocator.h"
#include "SoundSystem.h"

PookaComponent::PookaComponent(Twengine::GameObject* owner)
	: Component(owner)
{
	GetOwner()->SetTag(make_sdbm_hash("Enemy"));
	m_OnDeathEvent = std::make_unique<Twengine::Event>();
}

void PookaComponent::Start()
{
	Twengine::ServiceLocator::get_sound_system().RequestLoadSound("Level/EnemyPop.wav", make_sdbm_hash("EnemyPop"));
	Twengine::AnimationComponent* animationComp = GetOwner()->GetComponent<Twengine::AnimationComponent>();
	animationComp->AddAnimation("Pooka/PookaMove.png", make_sdbm_hash("PookaMove"), 2);
	animationComp->AddAnimation("Pooka/PookaPump.png", make_sdbm_hash("PookaPump"), 4, 3);
	animationComp->AddAnimation("Pooka/PookaGhost.png", make_sdbm_hash("PookaGhost"), 2);
	animationComp->AddAnimation("Pooka/PookaCrushed.png", make_sdbm_hash("PookaCrushed"), 1, 0);
	animationComp->PlayAnimation(make_sdbm_hash("PookaMove")); // To make sure we have a valid width and height for the hitbox
	Twengine::RectColliderComponent* rectColliderComponent = GetOwner()->GetComponent<Twengine::RectColliderComponent>();
	rectColliderComponent->SetHitBox(m_Transform->GetWorldPosition(), animationComp->GetAnimationFrameWidth(), animationComp->GetAnimationFrameHeight());
	rectColliderComponent->GetOnCollisionEvent()->AddObserver(this);
	m_CurrentState = std::make_unique<PookaIdleState>();
	m_CurrentState->OnEnter(GetOwner());
}

void PookaComponent::Update()
{
	CheckAndTransitionStates(m_CurrentState->Update(GetOwner()));
}

void PookaComponent::Notify(const GameEvent& event, Twengine::GameObject* observedObject)
{
	CheckAndTransitionStates(m_CurrentState->GetNotifiedByOwner(event, observedObject, GetOwner()));
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

