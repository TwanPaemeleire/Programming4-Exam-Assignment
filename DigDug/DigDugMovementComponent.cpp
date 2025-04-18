#include "DigDugMovementComponent.h"
#include "GameObject.h"
#include "MyTime.h"
#include "GameManager.h"
#include "GridComponent.h"
#include <iostream>
#include <algorithm>
#include "Renderer.h"
#include "GroundComponent.h"
#include "AnimationComponent.h"

DigDugMovementComponent::DigDugMovementComponent(Twengine::GameObject* owner)
	:Component(owner)
{
}

void DigDugMovementComponent::Start()
{
	m_GridComponent = GameManager::GetInstance().GetGrid();
	m_CurrentIndex = m_GridComponent->GetIndexFromPosition(m_Transform->GetWorldPosition());
	m_GroundComponent = GameManager::GetInstance().GetGround();
	m_AnimationComponent = GetOwner()->GetComponent<Twengine::AnimationComponent>();
}

void DigDugMovementComponent::Update()
{
	// Not Moving And No Input
	if (m_CurrentInputDirection == glm::vec2(0.f, 0.f) && !m_IsMoving)
	{
		return;
	}

	// Not Moving But Input Detected, So Pick A Target If None Has Been Defined Yet
	if (m_CurrentInputDirection != glm::vec2(0.f, 0.f) && m_TargetPosition == glm::vec2(-1.f, -1.f))
	{
		CalculateNextTarget();
	}

	// Move To Target
	if (m_IsMoving && m_CurrentInputDirection != glm::vec2(0.f, 0.f))
	{
		glm::vec2 newPos = m_Transform->GetWorldPosition();

		glm::vec2 moveDelta = m_Direction * m_MovementSpeed * Twengine::Time::GetInstance().deltaTime;
		m_DistanceTracker += (moveDelta.x + moveDelta.y);
		newPos += moveDelta;
		m_Transform->SetLocalPosition(newPos);

		UpdateGroundAndAnimation();

		// Target Reached, Allow Switching Of Directions
		if (abs(m_DistanceTracker) >= abs(m_DistanceToTarget))
		{
			m_Transform->SetLocalPosition(m_TargetPosition);
			CalculateNextTarget();
		}
	}
}

void DigDugMovementComponent::RenderUI()
{
	Twengine::Renderer::GetInstance().DrawPoint(m_TargetPosition.x, m_TargetPosition.y, SDL_Color(0, 0, 255), 5);
}

void DigDugMovementComponent::SetXDirection(float x)
{
	// No Input Detected At All
	if (x == 0.f && m_CurrentInputDirection.y == 0.f)
	{
		m_IsMoving = false;
		SetIdleAnimation();
	}
	if (x == 0.f) return;
	// Switch Orientations Left -> Right Or Right -> Left
	bool switchedOrientation = (m_CurrentInputDirection.x != 0.f && m_CurrentInputDirection.x != x);
	m_CurrentInputDirection = glm::vec2(x, 0.f);
	if (switchedOrientation) CalculateNextTarget();
	m_IsMoving = true;
}

void DigDugMovementComponent::SetYDirection(float y)
{
	// No Input Detected At All
	if (y == 0.f && m_CurrentInputDirection.x == 0.f)
	{
		m_IsMoving = false;
		SetIdleAnimation();
	}
	if (y == 0.f) return;
	// Switch Orientations Up -> Down Or Down -> Up
	bool switchedOrientation = (m_CurrentInputDirection.y != 0.f && m_CurrentInputDirection.y != y);
	m_CurrentInputDirection = glm::vec2(0.f, y);
	if (switchedOrientation) CalculateNextTarget();
	m_IsMoving = true;
}

void DigDugMovementComponent::CalculateNextTarget()
{
	m_CurrentIndex = m_GridComponent->GetIndexFromPosition(m_Transform->GetWorldPosition());

	std::pair<int, int> nextIndex = m_CurrentIndex;

	if (m_CurrentInputDirection.x < 0) --nextIndex.second;
	if (m_CurrentInputDirection.x > 0) ++nextIndex.second;
	if (m_CurrentInputDirection.y < 0) --nextIndex.first;
	if (m_CurrentInputDirection.y > 0) ++nextIndex.first;

	glm::vec2 currentCellPos = m_GridComponent->GetPositionFromIndex(m_CurrentIndex.first, m_CurrentIndex.second);
	glm::vec2 nextCellPosition = m_GridComponent->GetPositionFromIndex(nextIndex.first, nextIndex.second);
	glm::vec2 currentPosition = m_Transform->GetWorldPosition();

	float distanceToCurrentCell = glm::distance(currentPosition, currentCellPos);
	float distanceToNextCell = glm::distance(currentPosition, nextCellPosition);

	bool targetNext{};
	// Check If (Nearly) On Top Left Of Current Cell
	if (std::abs(currentPosition.x - currentCellPos.x) < 1e-3f && std::abs(currentPosition.y - currentCellPos.y) < 1e-3f)
	{
		targetNext = true;
	}
	else
	{
		// Closer To Top Left Of Current Cell Than Top Left Of Next Cell
		if (distanceToCurrentCell < distanceToNextCell)
		{
			// Closer To Current Cell, But Check If Current Cell Is Actually In The Direction We Want To Move Towards
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
		else // Closer To Top Left Of Next Cell Than Top Left Of Current Cell
		{
			targetNext = true;
		}
	}

	if (targetNext) // Target Next Cell
	{
		m_TargetPosition = nextCellPosition;
		m_DistanceToTarget = distanceToNextCell;
		m_CurrentIndex = nextIndex;
	}
	else // Target Current Cell
	{
		m_TargetPosition = currentCellPos;
		m_DistanceToTarget = distanceToCurrentCell;
		m_CurrentIndex = nextIndex;
	}

	m_Direction = m_CurrentInputDirection;
	UpdateFlipAndRotation();
	m_DistanceTracker = 0.f;
}

void DigDugMovementComponent::SetIdleAnimation()
{
	if (!m_HasStartedIdleAnimation)
	{
		m_HasStartedIdleAnimation = true;
		m_HasStartedDiggingAnimation = false;
		m_HasStartedWalkingAnimation = false;
		m_AnimationComponent->PlayAnimation(make_sdbm_hash("DigDugIdle"));
	}
}

void DigDugMovementComponent::UpdateGroundAndAnimation()
{
	SDL_Rect playerRect{};
	playerRect.x = static_cast<int>(m_Transform->GetWorldPosition().x);
	playerRect.y = static_cast<int>(m_Transform->GetWorldPosition().y);
	playerRect.w = static_cast<int>(m_AnimationComponent->GetAnimationFrameWidth());
	playerRect.h = static_cast<int>(m_AnimationComponent->GetAnimationFrameHeight());

	glm::vec2 posToCheck = m_Transform->GetWorldPosition();
	// Depending On Direction, We Need To Check A Different Position
	if (m_Direction == glm::vec2(1.f, 0.f)) // Moving Right
	{
		posToCheck.x += playerRect.w + 1.f;
	}
	else if (m_Direction == glm::vec2(-1.f, 0.f)) // Moving Left
	{
		posToCheck.x -= 1.f;
	}
	else if (m_Direction == glm::vec2(0.f, -1.f)) // Moving Up
	{
		posToCheck.y -= 1.f;
	}
	else if (m_Direction == glm::vec2(0.f, 1.f)) // Moving Down
	{
		posToCheck.y += playerRect.h + 1.f;
	}

	if (m_GroundComponent->PositionIsDugOut(posToCheck)) // Next Is Already Dug Out
	{
		if (!m_HasStartedWalkingAnimation)
		{
			m_HasStartedWalkingAnimation = true;
			m_HasStartedDiggingAnimation = false;
			m_HasStartedIdleAnimation = false;
			m_AnimationComponent->PlayAnimation(make_sdbm_hash("DigDugMove"));
		}
	}
	else // Next Is Not Dug Out Yet
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
}

void DigDugMovementComponent::UpdateFlipAndRotation()
{
	if (m_Direction.x > 0) // Moving Right
	{
		m_AnimationComponent->SetRotationAngle(0);
		m_AnimationComponent->SetFlipHorizontal(false);
	}
	else if (m_Direction.x < 0) // Moving Left
	{
		m_AnimationComponent->SetRotationAngle(0);
		m_AnimationComponent->SetFlipHorizontal(true);
	}
	else if (m_Direction.y > 0) // Moving Up
	{
		m_AnimationComponent->SetRotationAngle(90);
		m_AnimationComponent->SetFlipHorizontal(false);
	}
	else if (m_Direction.y < 0) // Moving Down
	{
		m_AnimationComponent->SetRotationAngle(90);
		m_AnimationComponent->SetFlipHorizontal(true);
	}
}
