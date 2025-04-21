#include "PookaComponent.h"
#include "Renderer.h"
#include "GameObject.h"
#include "AnimationComponent.h"
#include "GameManager.h"
#include "GroundComponent.h"
#include "MyTime.h"
#include "GridComponent.h"

PookaComponent::PookaComponent(Twengine::GameObject* owner)
	: Component(owner)
{
}

void PookaComponent::Start()
{
	m_Width = GetOwner()->GetComponent<Twengine::AnimationComponent>()->GetAnimationFrameWidth();
	m_Height = GetOwner()->GetComponent<Twengine::AnimationComponent>()->GetAnimationFrameHeight();
	m_GroundComponent = GameManager::GetInstance().GetGround();
	m_GridComponent = GameManager::GetInstance().GetGrid();
	m_GridCellSize = m_GridComponent->GetCellSize();

	m_IdleDirections[0].direction = { 1.f, 0.f };
	m_IdleDirections[1].direction = { 0.f, 1.f };
	m_IdleDirections[2].direction = { -1.f, 0.f };
	m_IdleDirections[3].direction = { 0.f, -1.f };

	SetNewIdleTarget();
}

void PookaComponent::Update()
{
	// Player Can't Be Reached
	if (m_NextNodeToPlayer == glm::vec2(-1.f, -1.f))
	{
		MovementIfNoPathToPlayer();
		return;
	}

	// Pathfinding To Player -> OPTIMIZE LATER
	glm::vec2 currentPosition = m_Transform->GetWorldPosition();
	glm::vec2 direction = m_NextNodeToPlayer - currentPosition;
	float distance = glm::length(direction);
	
	if (distance < 0.5f) // Next Node Reached
	{
		m_Transform->SetLocalPosition(m_NextNodeToPlayer);
		m_NextNodeToPlayer = GameManager::GetInstance().GetGround()->GetCellTargetToGetCloserToPlayer(m_Transform->GetWorldPosition());
	}
	else // Continue Moving Towards Next Node
	{
		glm::vec2 normalizedDir = glm::normalize(direction);
		glm::vec2 movement = normalizedDir * m_MovementSpeed * Twengine::Time::GetInstance().deltaTime;
		m_Transform->SetLocalPosition(currentPosition + movement);
	}	
}

void PookaComponent::RenderUI()
{
	Twengine::Renderer::GetInstance().DrawRectangle(m_NextNodeToPlayer.x, m_NextNodeToPlayer.y, m_Width, m_Height, SDL_Color(0, 255, 0, 255));
	Twengine::Renderer::GetInstance().DrawRectangle(m_IdleTarget.x, m_IdleTarget.y, 5.f, 5.f, SDL_Color(0, 0, 255, 255));
}

void PookaComponent::MovementIfNoPathToPlayer()
{
	glm::vec2 movement = m_IdleDirection * m_MovementSpeed * Twengine::Time::GetInstance().deltaTime;
	m_Transform->SetLocalPosition(glm::vec2(m_Transform->GetWorldPosition()) + movement);

	// When Reaching New Cell, Check If Path To Player Exists, Otherwise Pick New Idle Target
	if (glm::distance(glm::vec2(m_Transform->GetWorldPosition()), m_IdleTarget) < 1.f)
	{
		m_Transform->SetLocalPosition(m_IdleTarget);
		if (GameManager::GetInstance().GetGround()->EnemyCanReachPlayer(m_Transform->GetWorldPosition()))
		{
			m_NextNodeToPlayer = GameManager::GetInstance().GetGround()->GetCellTargetToGetCloserToPlayer(m_Transform->GetWorldPosition());
		}
		SetNewIdleTarget();
	}
}

void PookaComponent::SetNewIdleTarget()
{
	auto& pos = m_Transform->GetWorldPosition();

	// Offset Of 1 To Make Sure We're Checking Last Pixel Of Next Cell, And Not The First Of The One Behind That
	m_IdleDirections[0].target = glm::vec2(pos.x + (m_GridCellSize * 2) - 1.f, pos.y);
	m_IdleDirections[1].target = glm::vec2(pos.x, pos.y + (m_GridCellSize * 2) - 1.f);
	m_IdleDirections[2].target = glm::vec2(pos.x - m_GridCellSize + 1.f, pos.y);
	m_IdleDirections[3].target = glm::vec2(pos.x, pos.y - m_GridCellSize + 1.f);
	// Check If Previously Calculated Positions Are Reachable
	m_IdleDirections[0].canMoveHere = m_GroundComponent->CanMoveBetween(pos, m_IdleDirections[0].target);
	m_IdleDirections[1].canMoveHere = m_GroundComponent->CanMoveBetween(pos, m_IdleDirections[1].target);
	m_IdleDirections[2].canMoveHere = m_GroundComponent->CanMoveBetween(pos, m_IdleDirections[2].target);
	m_IdleDirections[3].canMoveHere = m_GroundComponent->CanMoveBetween(pos, m_IdleDirections[3].target);

	// Prioritize Previous Movement Direction If Possible
	if (m_PreviousDirIndex!= -1 && m_IdleDirections[m_PreviousDirIndex].canMoveHere)
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
