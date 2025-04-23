#include "EnemyMovementComponent.h"
#include "MyTime.h"
#include "GameObject.h"
#include "GameManager.h"
#include "GroundComponent.h"
#include "GridComponent.h"
#include "Event.h"
#include "Renderer.h"

EnemyMovementComponent::EnemyMovementComponent(Twengine::GameObject* owner)
	:Component(owner)
{
	m_CanReachPlayerEvent = std::make_unique<Twengine::Event>();
}

void EnemyMovementComponent::Start()
{
	m_GroundComponent = GameManager::GetInstance().GetGround();
	m_GridComponent = GameManager::GetInstance().GetGrid();
	m_GridCellSize = m_GridComponent->GetCellSize();

	m_IdleDirections[0].direction = { 1.f, 0.f };
	m_IdleDirections[1].direction = { 0.f, 1.f };
	m_IdleDirections[2].direction = { -1.f, 0.f };
	m_IdleDirections[3].direction = { 0.f, -1.f };

	SetNewIdleTarget();
}

void EnemyMovementComponent::RenderUI()
{
	Twengine::Renderer::GetInstance().DrawPoint(m_IdleTarget.x, m_IdleTarget.y, SDL_Color(0, 0, 255, 255), 5);
}


void EnemyMovementComponent::MovementIfNoPathToPlayer()
{
	glm::vec2 movement = m_IdleDirection * m_MovementSpeed * Twengine::Time::GetInstance().deltaTime;
	m_Transform->SetLocalPosition(glm::vec2(m_Transform->GetWorldPosition()) + movement);

	// When Reaching New Cell, Check If Path To Player Exists, Otherwise Pick New Idle Target
	if (glm::distance(glm::vec2(m_Transform->GetWorldPosition()), m_IdleTarget) < 1.f)
	{
		m_Transform->SetLocalPosition(m_IdleTarget);
		if (GameManager::GetInstance().GetGround()->EnemyCanReachPlayer(m_Transform->GetWorldPosition()))
		{
			m_CanReachPlayerEvent->NotifyObservers(GameEvent(make_sdbm_hash("OnCanReachPlayer")), GetOwner());
			//m_NextNodeToPlayer = GameManager::GetInstance().GetGround()->GetCellTargetToGetCloserToPlayer(m_Transform->GetWorldPosition());
		}
		SetNewIdleTarget();
	}
}

void EnemyMovementComponent::SetNewIdleTarget()
{
	auto& pos = m_Transform->GetWorldPosition();

	// Offset Of 1 To Make Sure We're Checking Last Pixel Of Next Cell, And Not The First Of The One Behind That
	m_IdleDirections[0].target = glm::vec2(pos.x + (m_GridCellSize * 2) /*- 1.f*/, pos.y);
	m_IdleDirections[1].target = glm::vec2(pos.x, pos.y + (m_GridCellSize * 2) /*- 1.f*/);
	m_IdleDirections[2].target = glm::vec2(pos.x - m_GridCellSize /*+ 1.f*/, pos.y);
	m_IdleDirections[3].target = glm::vec2(pos.x, pos.y - m_GridCellSize /*+ 1.f*/);

	// Targets To Check for
	glm::vec2 rightCheck = glm::vec2(m_IdleDirections[0].target.x - 1.f, m_IdleDirections[0].target.y);
	glm::vec2 downCheck = glm::vec2(m_IdleDirections[1].target.x, m_IdleDirections[1].target.y - 1.f);
	glm::vec2 leftCheck = glm::vec2(m_IdleDirections[2].target.x + 1.f, m_IdleDirections[2].target.y);
	glm::vec2 upCheck = glm::vec2(m_IdleDirections[3].target.x, m_IdleDirections[3].target.y + 1.f);

	// Check If Previously Calculated Positions Are Reachable
	m_IdleDirections[0].canMoveHere = m_GroundComponent->CanMoveBetween(pos, rightCheck);
	m_IdleDirections[1].canMoveHere = m_GroundComponent->CanMoveBetween(pos, downCheck);
	m_IdleDirections[2].canMoveHere = m_GroundComponent->CanMoveBetween(pos, leftCheck);
	m_IdleDirections[3].canMoveHere = m_GroundComponent->CanMoveBetween(pos, upCheck);

	// Prioritize Previous Movement Direction If Possible
	if (m_PreviousDirIndex != -1 && m_IdleDirections[m_PreviousDirIndex].canMoveHere)
	{
		m_IdleDirection = m_IdleDirections[m_PreviousDirIndex].direction;
		m_IdleTarget = m_IdleDirections[m_PreviousDirIndex].target;
		if (m_IdleDirections[m_PreviousDirIndex].direction == glm::vec2(1.f, 0.f))      m_IdleTarget.x -= m_GridCellSize;
		else if (m_IdleDirections[m_PreviousDirIndex].direction == glm::vec2(0.f, 1.f)) m_IdleTarget.y -= m_GridCellSize;
		return;
	}

	// Check Which Direction To Move In If Previous Wasn't Possible
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
