#include "PookaStates.h"
#include "AnimationComponent.h"
#include "EnemyMovementComponent.h"
#include "GameObject.h"

void PookaIdleState::OnEnter(Twengine::GameObject* stateOwner)
{
	m_MovementComp = stateOwner->GetComponent<EnemyMovementComponent>();
}

std::unique_ptr<PookaState> PookaIdleState::Update(Twengine::GameObject*)
{
	if (m_MovementComp->MovementIfNoPathToPlayer()) // Pooka Can Now Reach The Enemy
	{
		return std::make_unique<PookaTrackingState>();
	}
	return nullptr;
}

void PookaTrackingState::OnEnter(Twengine::GameObject* stateOwner)
{
	m_MovementComp = stateOwner->GetComponent<EnemyMovementComponent>();
}

std::unique_ptr<PookaState> PookaTrackingState::Update(Twengine::GameObject*)
{
	m_MovementComp->PathFindingToPlayer();
	return nullptr;
}