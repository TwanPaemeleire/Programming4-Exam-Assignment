#include "RockStates.h"
#include "GameManager.h"
#include "TransformComponent.h"
#include "GridComponent.h"
#include "GroundComponent.h"
#include "GameObject.h"
#include "RectColliderComponent.h"
#include "MyTime.h"
#include "AnimationComponent.h"
#include "TextureRenderComponent.h"
#include "Texture2D.h"

void RockIdleState::OnEnter(Twengine::GameObject* stateOwner)
{
	m_IndexUnderRock = GameManager::GetInstance().GetGrid()->GetIndexFromPosition(stateOwner->GetTransform()->GetWorldPosition());
	++m_IndexUnderRock.first;
	m_PositionToCheckIfFree = GameManager::GetInstance().GetGrid()->GetPositionFromIndex(m_IndexUnderRock);
	float halfCellSize = GameManager::GetInstance().GetGrid()->GetCellSize() / 2;
	m_PositionToCheckIfFree += glm::vec2(halfCellSize, halfCellSize);
	m_PlayerRectColliderComponent = GameManager::GetInstance().GetPlayerTransform()->GetOwner()->GetComponent<Twengine::RectColliderComponent>();
	stateOwner->GetComponent<Twengine::RectColliderComponent>()->SetEnabled(false);
}

std::unique_ptr<RockState> RockIdleState::Update(Twengine::GameObject*)
{
	if (!m_PlayerIsBeneath && GameManager::GetInstance().GetGround()->PositionIsDugOut(m_PositionToCheckIfFree))
	{
		m_PlayerIsBeneath = true;
	}
	else if(m_PlayerIsBeneath)
	{
		if (!PlayerIsUnderRock()) return std::make_unique<RockFallingState>();
	}
	return nullptr;
}

void RockIdleState::OnExit(Twengine::GameObject*)
{
	--m_IndexUnderRock.first;
	GameManager::GetInstance().GetGrid()->RemoveRockFromCell(m_IndexUnderRock);
}

bool RockIdleState::PlayerIsUnderRock()
{
	std::vector<Cell*> cells = GameManager::GetInstance().GetGrid()->GetCellsInRect(*m_PlayerRectColliderComponent->GetHitBox());
	auto it = std::find_if(cells.begin(), cells.end(), [&](const Cell* cell)
		{
			return GameManager::GetInstance().GetGrid()->GetIndexFromPosition(cell->topLeft) == m_IndexUnderRock;
		});
	if (it != cells.end()) // Player not found in cell beneath
	{
		return true;
	}
	return false;
}

void RockFallingState::OnEnter(Twengine::GameObject* stateOwner)
{
	m_GroundComponent = GameManager::GetInstance().GetGround();
	m_GridComponent = GameManager::GetInstance().GetGrid();
	m_Size = stateOwner->GetComponent<Twengine::TextureRenderComponent>()->GetTexture()->GetSize();

	glm::vec2 bottomCenter = stateOwner->GetTransform()->GetLocalPosition();
	bottomCenter += m_Size / 2.f;
	m_StartCellIndex = GameManager::GetInstance().GetGrid()->GetIndexFromPosition(bottomCenter);
	stateOwner->GetComponent<Twengine::RectColliderComponent>()->SetEnabled(true);
}

std::unique_ptr<RockState> RockFallingState::Update(Twengine::GameObject* stateOwner)
{
	auto newPos = stateOwner->GetTransform()->GetLocalPosition();
	newPos.x += m_Size.x / 2.f;
	newPos.y += m_Size.y;
	glm::vec2 bottomCenter = newPos;
	
	newPos.y += m_DropSpeed * Twengine::Time::GetInstance().deltaTime;


	m_HasLeftStartCell = (m_GridComponent->GetIndexFromPosition(bottomCenter) != m_StartCellIndex);
	if (m_HasLeftStartCell && !m_GroundComponent->CanMoveBetween(bottomCenter, newPos)) return std::make_unique<RockBreakingState>();
	newPos.x -= m_Size.x / 2.f;
	newPos.y -= m_Size.y;
	stateOwner->GetTransform()->SetLocalPosition(newPos);

	return nullptr;
}

void RockBreakingState::OnEnter(Twengine::GameObject* stateOwner)
{
	stateOwner->GetComponent<Twengine::RectColliderComponent>()->MarkForDestruction();
	stateOwner->GetComponent<Twengine::TextureRenderComponent>()->MarkForDestruction();
	m_AnimationComponent = stateOwner->GetComponent<Twengine::AnimationComponent>();
	m_AnimationComponent->AddAnimation("Level/RockBreaking.png", make_sdbm_hash("RockBreaking"), 3, 2);
	m_AnimationComponent->PlayAnimation(make_sdbm_hash("RockBreaking"), 0.5f);
}

std::unique_ptr<RockState> RockBreakingState::Update(Twengine::GameObject* stateOwner)
{
	if (m_AnimationComponent->HasFinishedPlayingOnce()) stateOwner->MarkForDestruction();
	return nullptr;
}