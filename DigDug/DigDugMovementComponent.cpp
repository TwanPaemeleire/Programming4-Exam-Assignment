#include "DigDugMovementComponent.h"
#include "GameObject.h"
#include "MyTime.h"
#include "GameManager.h"
#include "GridComponent.h"
#include <iostream>
#include <algorithm>
#include "Renderer.h"

DigDugMovementComponent::DigDugMovementComponent(Twengine::GameObject* owner)
	:Component(owner)
{
}

void DigDugMovementComponent::Start()
{
	m_GridComponent = GameManager::GetInstance().GetGrid();
	m_CurrentIndex = m_GridComponent->GetIndexFromPosition(m_Transform->GetWorldPosition());
}



// INPUT DETECTED
// CHECK IF THERE'S A TARGET -> if so move towards it, no matter what the input is, unless it's on the same axis, then we can freely switch between for example left and right so we should change target then
//							 -> if not, set the target based on movement input
// UPON REACHING CELL -> switch target based on desired direction

// For switching, we'll have to check with the closest point in the correct direction, this might be the cell we're already in
// We just need to make sure we're not picking the topleft of a cell, if we're already on the topleft of this cell
// But it might occur that we're halfway in the cell already when we start pressing that we want to go up, then when reaching the end of THIS cell, we want to go up, not only on the next one


void DigDugMovementComponent::Update()
{
	//std::cout << "X: " << m_CurrentInputDirection.x << " Y: " << m_CurrentInputDirection.y << std::endl;
	// Not Moving And No Input
	if (m_CurrentInputDirection == glm::vec2(0.f, 0.f) && !m_IsMoving)
	{
		return;
	}

	// Not Moving But Input Detected, So Pick A Target If None Has Been Defined Yet
	if (m_CurrentInputDirection != glm::vec2(0.f, 0.f) && m_TargetPosition == glm::vec2(-1.f, -1.f))
	{
		CalculateNextTarget();
		m_CanSwitchDirection = false;
	}

	// Move To Target
	if (m_IsMoving && m_CurrentInputDirection != glm::vec2(0.f, 0.f))
	{
		glm::vec2 newPos = m_Transform->GetWorldPosition();
		//std::cout << "NO MOVEMENT APPLIED YET: " << newPos.x << " " << newPos.y << std::endl;
		glm::vec2 moveDelta = m_Direction * m_MovementSpeed * Twengine::Time::GetInstance().deltaTime;
		m_DistanceTracker += (moveDelta.x + moveDelta.y);
		newPos += moveDelta;
		m_Transform->SetLocalPosition(newPos);
		//std::cout << "MOVEMENT APPLIED " << newPos.x << " " << newPos.y << std::endl;
		// Target Reached, Allow Switching Of Directions
		if (abs(m_DistanceTracker) >= abs(m_DistanceToTarget))
		{
			std::cout << "REACHED DEST" << std::endl;
			m_Transform->SetLocalPosition(m_TargetPosition);
			CalculateNextTarget();
		}
	}

	//std::cout << "distance traveled: " << m_DistanceTracker << std::endl;
	//std::cout << "total distance: " << m_DistanceToTarget << std::endl;
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
	}
	if (x == 0.f) return;
	m_CurrentInputDirection = glm::vec2(x, 0.f);
	m_IsMoving = true;
}

void DigDugMovementComponent::SetYDirection(float y)
{
	// No Input Detected At All
	if (y == 0.f && m_CurrentInputDirection.x == 0.f)
	{
		m_IsMoving = false;
	}
	if (y == 0.f) return;
	m_CurrentInputDirection = glm::vec2(0.f, y);
	m_IsMoving = true;
}

void DigDugMovementComponent::CalculateNextTarget()
{
	std::cout << "calculating next target" << std::endl;

	// find closest target in direction, might be in this cell, but make sure it's not already AT TOP LEFT
	std::pair<int, int> nextIndex = m_CurrentIndex;

	if (m_CurrentInputDirection.x < 0) --nextIndex.second;
	if (m_CurrentInputDirection.x > 0) ++nextIndex.second;
	if (m_CurrentInputDirection.y < 0) --nextIndex.first;
	if (m_CurrentInputDirection.y > 0) ++nextIndex.first;

	m_Direction = m_CurrentInputDirection;

	m_TargetPosition = m_GridComponent->GetPositionFromIndex(nextIndex.first, nextIndex.second);
	m_DistanceTracker = 0.f;
	m_DistanceToTarget = glm::distance(glm::vec2(m_Transform->GetWorldPosition()), m_TargetPosition);

	m_CurrentIndex = nextIndex;


	//glm::vec2 currentCellPos = m_GridComponent->GetPositionFromIndex(m_CurrentIndex.first, m_CurrentIndex.second);
	//glm::vec2 nextCellPosition = m_GridComponent->GetPositionFromIndex(nextIndex.first, nextIndex.second);
	//glm::vec2 currentPosition = m_Transform->GetWorldPosition();




	// Check If (Nearly) On Top Left Of Current Cell
	//if(!((std::abs(currentCellPos.x - currentPosition.x) < 1e-5f) && (std::abs(currentCellPos.y - currentPosition.y) < 1e-5f)))
	//{
	//	float distanceToCurrent = glm::distance(currentPosition, currentCellPos);
	//	float absDistanceToCurrent = abs(distanceToCurrent);
	//	float distanceToNext = glm::distance(currentPosition, nextCellPosition);
	//	float absDistanceToNext = abs(distanceToNext);
	//
	//	// Closer To Current Cell Top Left Then Supposed Next Cell Top Left
	//	if (absDistanceToCurrent > absDistanceToNext) // Closer To Next Cell
	//	{
	//		m_TargetPosition = nextCellPosition;
	//		m_DistanceToTarget = absDistanceToNext;
	//	}
	//	else // Closer To Current Cell
	//	{
	//		m_TargetPosition = currentCellPos;
	//		m_DistanceToTarget = absDistanceToCurrent;
	//	}
	//}
	//else // You're Perfectly On Top Of Current Cell -> So Move To Next
	//{
	//	std::cout << " ON EDGE" << std::endl;
	//	float distanceToCurrent = glm::distance(currentPosition, currentCellPos);
	//	float absDistanceToCurrent = abs(distanceToCurrent);
	//	m_TargetPosition = nextCellPosition;
	//	m_DistanceToTarget = absDistanceToCurrent;
	//}
	//m_DistanceTracker = 0.f;
}