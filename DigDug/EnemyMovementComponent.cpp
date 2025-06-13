#include "EnemyMovementComponent.h"
#include "MyTime.h"
#include "GameObject.h"
#include "GameManager.h"
#include "GroundComponent.h"
#include "GridComponent.h"
#include "RectColliderComponent.h"
#include "Event.h"
#include "Renderer.h"

EnemyMovementComponent::EnemyMovementComponent(Twengine::GameObject* owner)
	:Component(owner)
{
}

void EnemyMovementComponent::Start()
{
	m_GroundComponent = GameManager::GetInstance().GetGround();
	m_GridComponent = GameManager::GetInstance().GetGrid();
	m_RectColliderComponent = GetOwner()->GetComponent<Twengine::RectColliderComponent>();
	m_GridCellSize = m_GridComponent->GetCellSize();
	m_HalfGridCellSize = m_GridCellSize / 2.f;

	m_IdleDirections[0].direction = { 1.f, 0.f };
	m_IdleDirections[1].direction = { 0.f, 1.f };
	m_IdleDirections[2].direction = { -1.f, 0.f };
	m_IdleDirections[3].direction = { 0.f, -1.f };

	SetNewIdleTarget();
	ResetGhostStateValues();
}

void EnemyMovementComponent::Update()
{
	if (m_GhostCooldownHasFinished) return;
	m_GhostCoolDownTimer += Twengine::Time::GetInstance().deltaTime;
	if (m_GhostCoolDownTimer >= m_GhostCooldown)
	{
		m_GhostCooldownHasFinished = true;
	}
}

void EnemyMovementComponent::RenderUI()
{
	//Twengine::Renderer::GetInstance().DrawPoint(m_IdleTarget.x, m_IdleTarget.y, SDL_Color(0, 0, 255, 255), 5);
	//Twengine::Renderer::GetInstance().DrawRectangle(m_NextNodeToPlayer.x, m_NextNodeToPlayer.y, 5.f, 5.f, SDL_Color(0, 255, 0, 255));
}

bool EnemyMovementComponent::MovementIfNoPathToPlayer()
{
	const glm::vec2 movement = m_IdleDirection * m_MovementSpeed * Twengine::Time::GetInstance().deltaTime;
	m_Transform->SetLocalPosition(glm::vec2(m_Transform->GetWorldPosition()) + movement);

	// When reaching new cell, check if path to player exists, otherwise pick new idle target
	if (glm::distance(glm::vec2(m_Transform->GetWorldPosition()), m_IdleTarget) < 1.f)
	{
		m_Transform->SetLocalPosition(m_IdleTarget);
		if (GameManager::GetInstance().GetGround()->EnemyCanReachPlayer(m_Transform->GetWorldPosition()))
		{
			m_NextNodeToPlayer = m_GroundComponent->GetCellTargetToGetCloserToPlayer(m_Transform->GetWorldPosition());
			return true;
		}
		SetNewIdleTarget();
	}
	return false;
}

bool EnemyMovementComponent::MovementInGhostForm()
{
	m_GhostFormTimer += Twengine::Time::GetInstance().deltaTime;

	if (!m_IsGettingPositionedInCell)
	{
		// Move enemy towards player
		const glm::vec2& currentPos = GetOwner()->GetTransform()->GetWorldPosition();
		const glm::vec2& playerPos = GameManager::GetInstance().GetClosestPlayerTransform(currentPos)->GetWorldPosition();
		const glm::vec2 direction = glm::normalize(playerPos - currentPos);
		const glm::vec2 newPos = currentPos + direction * m_MovementSpeed * Twengine::Time::GetInstance().deltaTime;
		GetOwner()->GetTransform()->SetLocalPosition(newPos);

		if (m_GhostFormTimer >= m_MinimumTimeInGhostForm)
		{
			GridComponent* gridComponent = GameManager::GetInstance().GetGrid();
			Twengine::RectHitbox* hitBox = m_RectColliderComponent->GetHitBox();
			SDL_Rect enemyRect = SDL_Rect(static_cast<int>(hitBox->topLeft.x), static_cast<int>(hitBox->topLeft.y),
										  static_cast<int>(hitBox->width), static_cast<int>(hitBox->height));
			std::vector<Cell*> nearbyCells = gridComponent->GetCellsInRect(enemyRect);
			for(Cell* const cell : nearbyCells) // Check if any of the nearby cells are walkable
			{
				const glm::vec2 cellCenter = glm::vec2(cell->topLeft.x + m_HalfGridCellSize, cell->topLeft.y + m_HalfGridCellSize);
				if (m_GroundComponent->PositionIsDugOut(cellCenter))
				{
					m_CellToPositionIn = cell->topLeft;
					m_IsGettingPositionedInCell = true;
					break;
				}
			}
		}
		return false;
	}
	else // Move towards the targeted cell
	{
		const glm::vec2& currentPos = GetOwner()->GetTransform()->GetWorldPosition();
		const glm::vec2 direction = glm::normalize(m_CellToPositionIn - currentPos);
		glm::vec2 newPos = currentPos + direction * m_MovementSpeed * Twengine::Time::GetInstance().deltaTime;

		if (glm::distance(newPos, m_CellToPositionIn) < 1.0f) // Targeted cell reached
		{
			m_GhostFormTimer = 0.f;
			m_IsGettingPositionedInCell = false;
			GetOwner()->GetTransform()->SetLocalPosition(m_CellToPositionIn);
			return true;
		}

		GetOwner()->GetTransform()->SetLocalPosition(newPos);
		return false;
	}
}

void EnemyMovementComponent::SetNewIdleTarget()
{
	const auto& pos = m_Transform->GetWorldPosition();

	m_IdleDirections[0].target = glm::vec2(pos.x + (m_GridCellSize * 2), pos.y);
	m_IdleDirections[1].target = glm::vec2(pos.x, pos.y + (m_GridCellSize * 2));
	m_IdleDirections[2].target = glm::vec2(pos.x - m_GridCellSize, pos.y);
	m_IdleDirections[3].target = glm::vec2(pos.x, pos.y - m_GridCellSize);

	// Offset of 1 to make sure we're checking last pixel of next cell, and not the first of the one behind that
	glm::vec2 rightCheck = glm::vec2(m_IdleDirections[0].target.x - 1.f, m_IdleDirections[0].target.y);
	glm::vec2 downCheck = glm::vec2(m_IdleDirections[1].target.x, m_IdleDirections[1].target.y - 1.f);
	glm::vec2 leftCheck = glm::vec2(m_IdleDirections[2].target.x + 1.f, m_IdleDirections[2].target.y);
	glm::vec2 upCheck = glm::vec2(m_IdleDirections[3].target.x, m_IdleDirections[3].target.y + 1.f);

	// Check if previously calculated positions are reachable
	m_IdleDirections[0].canMoveHere = m_GroundComponent->CanMoveBetween(pos, rightCheck);
	m_IdleDirections[1].canMoveHere = m_GroundComponent->CanMoveBetween(pos, downCheck);
	m_IdleDirections[2].canMoveHere = m_GroundComponent->CanMoveBetween(pos, leftCheck);
	m_IdleDirections[3].canMoveHere = m_GroundComponent->CanMoveBetween(pos, upCheck);

	// Prioritize previous movement direction if possible
	if (m_PreviousDirIndex != -1 && m_IdleDirections[m_PreviousDirIndex].canMoveHere)
	{
		m_IdleDirection = m_IdleDirections[m_PreviousDirIndex].direction;
		m_IdleTarget = m_IdleDirections[m_PreviousDirIndex].target;
		if (m_IdleDirections[m_PreviousDirIndex].direction == glm::vec2(1.f, 0.f))      m_IdleTarget.x -= m_GridCellSize;
		else if (m_IdleDirections[m_PreviousDirIndex].direction == glm::vec2(0.f, 1.f)) m_IdleTarget.y -= m_GridCellSize;
		return;
	}

	// Check which direction to move in if previous wasn't possible
	for (size_t dirCounter = 0; dirCounter < m_IdleDirections.size(); ++dirCounter)
	{
		if (m_IdleDirections[dirCounter].canMoveHere)
		{
			m_IdleTarget = m_IdleDirections[dirCounter].target;
			if (m_IdleDirections[dirCounter].direction == glm::vec2(1.f, 0.f))      m_IdleTarget.x -= m_GridCellSize;
			else if (m_IdleDirections[dirCounter].direction == glm::vec2(0.f, 1.f)) m_IdleTarget.y -= m_GridCellSize;
			m_IdleDirection = m_IdleDirections[dirCounter].direction;
			m_PreviousDirIndex = static_cast<int>(dirCounter);
			return;
		}
	}
}

void EnemyMovementComponent::PathFindingToPlayer()
{
	// Check if a target was already set
	if (m_NextNodeToPlayer == glm::vec2(-1.f, -1.f))
	{
		m_NextNodeToPlayer = m_GroundComponent->GetCellTargetToGetCloserToPlayer(m_Transform->GetWorldPosition());
	}
	// Pathfinding to player
	const glm::vec2& currentPosition = m_Transform->GetWorldPosition();
	const glm::vec2 direction = m_NextNodeToPlayer - currentPosition;
	const float distance = glm::length(direction);

	if (distance < 0.5f) // Next node reached
	{
		m_Transform->SetLocalPosition(m_NextNodeToPlayer);
		m_NextNodeToPlayer = m_GroundComponent->GetCellTargetToGetCloserToPlayer(m_Transform->GetWorldPosition());
	}
	else // Continue moving towards next node
	{
		const glm::vec2 normalizedDir = glm::normalize(direction);
		const glm::vec2 movement = normalizedDir * m_MovementSpeed * Twengine::Time::GetInstance().deltaTime;
		m_Transform->SetLocalPosition(currentPosition + movement);
	}
}

void EnemyMovementComponent::ResetGhostStateValues()
{
	m_GhostCooldownHasFinished = false;
	m_IsGettingPositionedInCell = false;
	m_GhostFormTimer = 0.0f;
	m_GhostCoolDownTimer = 0.0f;
	m_GhostCooldown = rand() % static_cast<int>(m_MaximumGhostFormCooldown - m_MinimumGhostFormCooldown) + m_MinimumGhostFormCooldown;
}