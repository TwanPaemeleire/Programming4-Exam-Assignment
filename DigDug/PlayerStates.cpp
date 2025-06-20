#include "PlayerStates.h"
#include "GameObject.h"
#include "AnimationComponent.h"
#include "DigDugPumpComponent.h"
#include "DigDugComponent.h"
#include "GridComponent.h"
#include "GroundComponent.h"
#include "GameManager.h"
#include "MyTime.h"
#include "Renderer.h"
#include "SceneManager.h"
#include <memory>
#include <iostream>
#include <algorithm>
#include "LivesComponent.h"
#include "ServiceLocator.h"
#include "SoundSystem.h"
#include "FygarComponent.h"
#include "PookaComponent.h"
#include "RockComponent.h"
#include "RectColliderComponent.h"

void PlayerMoving::OnEnter(Twengine::GameObject* stateOwner)
{
	PlayerMovingData* movingData = stateOwner->GetComponent<DigDugComponent>()->GetPlayerMovingData();

	m_Direction = movingData->direction;
	m_LastNonNullDirection = movingData->lastNonNullDirection;
	m_TargetPosition = movingData->targetPosition;
	m_DistanceToTarget = movingData->distanceToTarget;
	m_DistanceTracker = movingData->distanceTracker;
	m_CurrentIndex = movingData->currentIndex;
	m_IsMoving = movingData->isMoving;

	m_AnimationComponent = stateOwner->GetComponent<Twengine::AnimationComponent>();
	if(!m_AnimationComponent->IsPlayingAnimation(make_sdbm_hash("DigDugIdle")))
	{ 
		m_AnimationComponent->PlayAnimation(make_sdbm_hash("DigDugIdle"));
	}
	m_GridComponent = GameManager::GetInstance().GetGrid();
	m_GroundComponent = GameManager::GetInstance().GetGround();
	m_Transform = stateOwner->GetTransform();
	m_CurrentIndex = m_GridComponent->GetIndexFromPosition(m_Transform->GetWorldPosition());
}

std::unique_ptr<PlayerState> PlayerMoving::Update(Twengine::GameObject*)
{
	if (m_ShouldStartPumping)
	{
		return std::make_unique<PlayerPumpingState>(m_LastNonNullDirection);
	}

	// Not moving and no input
	if (m_CurrentInputDirection == glm::vec2(0.f, 0.f) && !m_IsMoving)
	{
		return nullptr;
	}

	// Not moving but input detected, so pick a target if none has been defined yet
	if (m_CurrentInputDirection != glm::vec2(0.f, 0.f) && m_TargetPosition == glm::vec2(-1.f, -1.f))
	{
		CalculateNextTarget();
	}

	// Move to target
	if (m_IsMoving && m_CurrentInputDirection != glm::vec2(0.f, 0.f))
	{
		glm::vec2 newPos = m_Transform->GetWorldPosition();

		const glm::vec2 moveDelta = m_Direction * m_MovementSpeed * Twengine::Time::GetInstance().deltaTime;
		m_DistanceTracker += (moveDelta.x + moveDelta.y);
		newPos += moveDelta;
		m_Transform->SetLocalPosition(newPos);

		UpdateGroundAndAnimation();

		// Target reached, allow switching of directions
		if (abs(m_DistanceTracker) >= abs(m_DistanceToTarget))
		{
			m_Transform->SetLocalPosition(m_TargetPosition);
			CalculateNextTarget();
		}
	}
	return nullptr;
}

void PlayerMoving::OnExit(Twengine::GameObject* stateOwner)
{
	PlayerMovingData* movingData = stateOwner->GetComponent<DigDugComponent>()->GetPlayerMovingData();

	movingData->direction = m_Direction;
	movingData->lastNonNullDirection = m_LastNonNullDirection;
	movingData->targetPosition = m_TargetPosition;
	movingData->currentInputDirection = m_CurrentInputDirection;
	movingData->distanceToTarget = m_DistanceToTarget;
	movingData->distanceTracker = m_DistanceTracker;
	movingData->currentIndex = m_CurrentIndex;
	movingData->isMoving = m_IsMoving;
}

void PlayerMoving::RenderDebugDrawing() const
{
	//Twengine::Renderer::GetInstance().DrawPoint(m_TargetPosition.x, m_TargetPosition.y, SDL_Color(0, 0, 255), 5);
}

std::unique_ptr<PlayerState> PlayerMoving::SetXDirection(Twengine::GameObject*, float x, float yToSet)
{
	// No input detected at all
	if (x == 0.f && m_CurrentInputDirection.y == 0.f && yToSet == 0.f)
	{
		m_IsMoving = false;
		SetIdleAnimation();
	}
	if (x == 0.f) return nullptr;
	// Switch orientations left -> right or right -> left
	const bool switchedOrientation = (m_CurrentInputDirection.x != 0.f && m_CurrentInputDirection.x != x && m_Direction.y == 0.f);
	m_CurrentInputDirection = glm::vec2(x, 0.f);
	if (switchedOrientation) CalculateNextTarget();
	m_IsMoving = true;
	return nullptr;
}

std::unique_ptr<PlayerState> PlayerMoving::SetYDirection(Twengine::GameObject*, float y, float xToSet)
{
	// No input detected at all
	if (y == 0.f && m_CurrentInputDirection.x == 0.f && xToSet == 0.f)
	{
		m_IsMoving = false;
		SetIdleAnimation();
	}
	if (y == 0.f) return nullptr;
	// Switch orientations up -> down or down -> up
	const bool switchedOrientation = (m_CurrentInputDirection.y != 0.f && m_CurrentInputDirection.y != y && m_Direction.x == 0.f);
	m_CurrentInputDirection = glm::vec2(0.f, y);
	if (switchedOrientation) CalculateNextTarget();
	m_IsMoving = true;
	return nullptr;
}

std::unique_ptr<PlayerState> PlayerMoving::OnPumpButtonInteraction(Twengine::GameObject*, Twengine::InteractionStates interactionState)
{
	if (interactionState == Twengine::InteractionStates::down)
	{
		m_ShouldStartPumping = true;
	}
	return nullptr;
}

std::unique_ptr<PlayerState> PlayerMoving::Notify(Twengine::GameObject* observedObject, const GameEvent& event)
{
	if (event.id == make_sdbm_hash("OnCollisionEnter") && (observedObject->GetTag() == make_sdbm_hash("Enemy") || observedObject->GetTag() == make_sdbm_hash("FygarFire")))
	{
		auto* fygarComp = observedObject->GetComponent<FygarComponent>();
		if (fygarComp && fygarComp->IsBeingPumped()) return nullptr;
		auto* pookaComp = observedObject->GetComponent<PookaComponent>();
		if (pookaComp && pookaComp->IsBeingPumped()) return nullptr;
		return std::make_unique<PlayerDeathState>();
	}
	else if (event.id == make_sdbm_hash("OnCollisionEnter") && observedObject->GetTag() == make_sdbm_hash("Rock"))
	{
		RockComponent* rock = observedObject->GetComponent<RockComponent>();
		return std::make_unique<PlayerRockDraggingState>(rock);
	}
	return nullptr;
}

void PlayerMoving::CalculateNextTarget()
{
	m_CurrentIndex = m_GridComponent->GetIndexFromPosition(m_Transform->GetWorldPosition());

	std::pair<int, int> nextIndex = m_CurrentIndex;

	if (m_CurrentInputDirection.x < 0) --nextIndex.second;
	if (m_CurrentInputDirection.x > 0) ++nextIndex.second;
	if (m_CurrentInputDirection.y < 0) --nextIndex.first;
	if (m_CurrentInputDirection.y > 0) ++nextIndex.first;

	nextIndex = m_GridComponent->ClampToPlayfieldIndex(nextIndex.first, nextIndex.second);
	if (m_GridComponent->IndexHoldsRock(nextIndex)) return;

	const glm::vec2 currentCellPos = m_GridComponent->GetPositionFromIndex(m_CurrentIndex.first, m_CurrentIndex.second);
	const glm::vec2 nextCellPosition = m_GridComponent->GetPositionFromIndex(nextIndex.first, nextIndex.second);
	const glm::vec2 currentPosition = m_Transform->GetWorldPosition();

	const float distanceToCurrentCell = glm::distance(currentPosition, currentCellPos);
	const float distanceToNextCell = glm::distance(currentPosition, nextCellPosition);

	bool targetNext{};
	// Check if (nearly) on top left of current cell
	if (std::abs(currentPosition.x - currentCellPos.x) < 1e-3f && std::abs(currentPosition.y - currentCellPos.y) < 1e-3f)
	{
		targetNext = true;
	}
	else
	{
		// Closer to top left of current cell than top left of next cell
		if (distanceToCurrentCell < distanceToNextCell)
		{
			// Closer to current cell, but check if current cell is actually in the direction we want to move towards
			if ((m_CurrentInputDirection.x < 0 && currentPosition.x < currentCellPos.x) ||
				(m_CurrentInputDirection.x > 0 && currentPosition.x > currentCellPos.x) ||
				(m_CurrentInputDirection.y < 0 && currentPosition.y < currentCellPos.y) ||
				(m_CurrentInputDirection.y > 0 && currentPosition.y > currentCellPos.y))
			{
				targetNext = true;
			}
			else
			{
				targetNext = false;
			}
		}
		else // Closer to next cell than current cell
		{
			targetNext = true;
		}
	}

	if (targetNext) // Target next cell
	{
		m_TargetPosition = nextCellPosition;
		m_DistanceToTarget = distanceToNextCell;
		m_CurrentIndex = nextIndex;
	}
	else // Target current cell
	{
		m_TargetPosition = currentCellPos;
		m_DistanceToTarget = distanceToCurrentCell;
		m_CurrentIndex = nextIndex;
	}

	m_Direction = m_CurrentInputDirection;
	m_LastNonNullDirection = m_Direction;
	UpdateFlipAndRotation();
	m_DistanceTracker = 0.f;
}

void PlayerMoving::SetIdleAnimation()
{
	if (!m_HasStartedIdleAnimation)
	{
		m_HasStartedIdleAnimation = true;
		m_HasStartedDiggingAnimation = false;
		m_HasStartedWalkingAnimation = false;
		m_AnimationComponent->PlayAnimation(make_sdbm_hash("DigDugIdle"));
	}
}

void PlayerMoving::UpdateGroundAndAnimation()
{
	SDL_Rect playerRect{};
	playerRect.x = static_cast<int>(m_Transform->GetWorldPosition().x);
	playerRect.y = static_cast<int>(m_Transform->GetWorldPosition().y);
	playerRect.w = static_cast<int>(m_AnimationComponent->GetAnimationFrameWidth());
	playerRect.h = static_cast<int>(m_AnimationComponent->GetAnimationFrameHeight());

	glm::vec2 posToCheck = m_Transform->GetWorldPosition();
	// Depending on direction, we need to check a different position
	if (m_Direction == glm::vec2(1.f, 0.f)) // Moving right
	{
		posToCheck.x += playerRect.w + 1.f;
	}
	else if (m_Direction == glm::vec2(-1.f, 0.f)) // Moving left
	{
		posToCheck.x -= 1.f;
	}
	else if (m_Direction == glm::vec2(0.f, -1.f)) // Moving up
	{
		posToCheck.y -= 1.f;
	}
	else if (m_Direction == glm::vec2(0.f, 1.f)) // Moving down
	{
		posToCheck.y += playerRect.h + 1.f;
	}

	if (m_GroundComponent->PositionIsDugOut(posToCheck)) // Next is already dug out
	{
		if (!m_HasStartedWalkingAnimation)
		{
			m_HasStartedWalkingAnimation = true;
			m_HasStartedDiggingAnimation = false;
			m_HasStartedIdleAnimation = false;
			m_AnimationComponent->PlayAnimation(make_sdbm_hash("DigDugMove"));
		}
	}
	else // Next is not dug out yet
	{
		if (!m_HasStartedDiggingAnimation)
		{
			m_HasStartedDiggingAnimation = true;
			m_HasStartedWalkingAnimation = false;
			m_HasStartedIdleAnimation = false;
			m_AnimationComponent->PlayAnimation(make_sdbm_hash("DigDugDigging"));
		}
	}
	m_GroundComponent->ErasePlayerTrail(playerRect);
	// If time left later down the line, do this, and make player and enemies be at center of cell instead of top left 
	// Will give smoother look for entire game but require a lot of reworking 
	//int centerX = static_cast<int>(m_Transform->GetWorldPosition().x);
	//int width = static_cast<int>(GetOwner()->GetComponent<Twengine::AnimationComponent>()->GetAnimationFrameWidth());
	//centerX += width / 2;
	//int centerY = static_cast<int>(m_Transform->GetWorldPosition().y);
	//int height = static_cast<int>(GetOwner()->GetComponent<Twengine::AnimationComponent>()->GetAnimationFrameHeight());
	//centerY += height / 2;
	//m_GroundComponent->ErasePlayerTrail(centerX, centerY, width, height);
}

void PlayerMoving::UpdateFlipAndRotation()
{
	if(m_Direction.x != 0.f)
	{
		m_AnimationComponent->SetRotationAngle(0);
		m_AnimationComponent->SetFlipHorizontal((m_Direction.x < 0));
	}
	else if (m_Direction.y != 0.f)
	{
		m_AnimationComponent->SetRotationAngle(90);
		m_AnimationComponent->SetFlipHorizontal((m_Direction.y < 0));
	}
	//if (m_Direction.x > 0) // Moving right
	//{
	//	m_AnimationComponent->SetRotationAngle(0);
	//	m_AnimationComponent->SetFlipHorizontal(false);
	//}
	//else if (m_Direction.x < 0) // Moving left
	//{
	//	m_AnimationComponent->SetRotationAngle(0);
	//	m_AnimationComponent->SetFlipHorizontal(true);
	//}
	//else if (m_Direction.y > 0) // Moving up
	//{
	//	m_AnimationComponent->SetRotationAngle(90);
	//	m_AnimationComponent->SetFlipHorizontal(false);
	//}
	//else if (m_Direction.y < 0) // Moving down
	//{
	//	m_AnimationComponent->SetRotationAngle(90);
	//	m_AnimationComponent->SetFlipHorizontal(true);
	//}
}

PlayerPumpingState::PlayerPumpingState(const glm::vec2& facingDir)
	:m_FacingDirection{facingDir}
{
}

void PlayerPumpingState::OnEnter(Twengine::GameObject* stateOwner)
{
	Twengine::AnimationComponent* animationComponent = stateOwner->GetComponent<Twengine::AnimationComponent>();
	animationComponent->PlayAnimation(make_sdbm_hash("DigDugPump"), 0.0f, false);
	
	std::unique_ptr<Twengine::GameObject> pump = std::make_unique<Twengine::GameObject>();
	m_DigDugPumpComponent = pump->AddComponent<DigDugPumpComponent>();
	m_DigDugPumpComponent->GetOnPumpRetractedEvent()->AddObserver(stateOwner->GetComponent<DigDugComponent>());
	
	SetPositionAndDirectionOfPump(stateOwner, pump.get(), 
								animationComponent->GetAnimationFrameWidth(), 
								animationComponent->GetAnimationFrameHeight());
	
	Twengine::SceneManager::GetInstance().GetCurrentScene().Add(std::move(pump));
}

std::unique_ptr<PlayerState> PlayerPumpingState::Update(Twengine::GameObject*)
{
	return nullptr;
}

std::unique_ptr<PlayerState> PlayerPumpingState::OnPumpButtonInteraction(Twengine::GameObject*, Twengine::InteractionStates interactionState)
{
	const bool pressBound = (interactionState == Twengine::InteractionStates::down || interactionState == Twengine::InteractionStates::pressed);
	m_DigDugPumpComponent->OnPumpButtonInteraction(pressBound);
	return nullptr;
}

std::unique_ptr<PlayerState> PlayerPumpingState::Notify(Twengine::GameObject* observedObject, const GameEvent& event)
{
	if (event.id == make_sdbm_hash("OnPumpRetracted"))
	{
		return std::make_unique<PlayerMoving>();
	}
	else if (event.id == make_sdbm_hash("OnCollisionEnter") && observedObject->GetTag() == make_sdbm_hash("Rock"))
	{
		RockComponent* rock = observedObject->GetComponent<RockComponent>();
		return std::make_unique<PlayerRockDraggingState>(rock);
	}
	return nullptr;
}

void PlayerPumpingState::SetPositionAndDirectionOfPump(Twengine::GameObject* stateOwner, Twengine::GameObject* pumpObject, float frameWidth, float frameHeight)
{
	const auto& stateOwnerPos = stateOwner->GetTransform()->GetWorldPosition();
	glm::vec2 posToSet{stateOwnerPos};
	bool flippedHorizontal{};
	bool vertical{};
	if (m_FacingDirection.x != 0.f)
	{
		if (m_FacingDirection.x == 1.f) posToSet.x += frameWidth;
		posToSet.y += frameHeight / 2.f;
		flippedHorizontal = (m_FacingDirection.x == -1.f);
		vertical = false;
	}
	else if (m_FacingDirection.y != 0.f)
	{
		posToSet.x += frameWidth / 2.f;
		if (m_FacingDirection.y == 1.f) posToSet.y += frameHeight;
		flippedHorizontal = (m_FacingDirection.y == -1.f);
		vertical = true;
	}

	if (flippedHorizontal) m_DigDugPumpComponent->FlipHorizontally();
	if (vertical) m_DigDugPumpComponent->ShootVertically();
	pumpObject->GetTransform()->SetLocalPosition(posToSet);
}

void PlayerDeathState::OnEnter(Twengine::GameObject* stateOwner)
{
	m_AnimationComponent = stateOwner->GetComponent<Twengine::AnimationComponent>();
	m_AnimationComponent->PlayAnimation(make_sdbm_hash("DigDugDeath"), 0.5f);
	Twengine::ServiceLocator::get_sound_system().RequestPlaySound(make_sdbm_hash("DigDugDeath"), 0.2f);
}


std::unique_ptr<PlayerState> PlayerDeathState::Update(Twengine::GameObject* stateOwner)
{
	if (m_AnimationComponent->HasFinishedPlayingOnce())
	{
		stateOwner->GetComponent<LivesComponent>()->Kill();
	}
	return nullptr;
}

PlayerRockDraggingState::PlayerRockDraggingState(RockComponent* rockComponent)
	:m_RockComponent{rockComponent}
{
	m_EnemyCrushedEvent = std::make_unique<Twengine::Event>();
	m_EnemyCrushedEvent->AddObserver(rockComponent);
}

void PlayerRockDraggingState::OnEnter(Twengine::GameObject* stateOwner)
{
	m_AnimationComponent = stateOwner->GetComponent<Twengine::AnimationComponent>();
	m_AnimationComponent->SetRotationAngle(0.0);
	m_AnimationComponent->SetFlipHorizontal(false);
	m_AnimationComponent->SetFlipVertical(false);
	m_AnimationComponent->PlayAnimation(make_sdbm_hash("DigDugCrushed"), 0.f, false);
	stateOwner->SetParent(m_RockComponent->GetOwner(), true);
	m_AmountUnderRockToCheck = m_AnimationComponent->GetAnimationFrameHeight();
	stateOwner->GetComponent<Twengine::RectColliderComponent>()->Disable();
}

std::unique_ptr<PlayerState> PlayerRockDraggingState::Update(Twengine::GameObject* stateOwner)
{
	glm::vec2 posToCheck = m_RockComponent->GetBottomMiddle();
	posToCheck.y += m_AmountUnderRockToCheck;
	if (!GameManager::GetInstance().GetGround()->PositionIsDugOut(posToCheck))
	{
		stateOwner->GetComponent<Twengine::RectColliderComponent>()->Enable();
		m_EnemyCrushedEvent->NotifyObservers(GameEvent(make_sdbm_hash("OnEnemyCrushed")), stateOwner);
		return std::make_unique<PlayerDeathState>();
	}
	return nullptr;
}