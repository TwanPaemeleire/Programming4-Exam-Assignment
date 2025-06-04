#include "RockComponent.h"
#include "GameManager.h"
#include "GridComponent.h"
#include "TransformComponent.h"
#include "GroundComponent.h"
#include "RectColliderComponent.h"
#include "GameObject.h"
#include "MyTime.h"
#include "AnimationComponent.h"
#include "TextureRenderComponent.h"
#include "Texture2D.h"
#include "ScoreComponent.h"
#include <glm.hpp>

RockComponent::RockComponent(Twengine::GameObject* owner)
	:Component(owner)
{
	owner->SetTag(make_sdbm_hash("Rock"));
}

void RockComponent::Start()
{
	Twengine::TextureRenderComponent* textureRenderComponent = GetOwner()->GetComponent<Twengine::TextureRenderComponent>();
	glm::ivec2 size = textureRenderComponent->GetTexture()->GetSize();
	GetOwner()->GetComponent<Twengine::RectColliderComponent>()->SetHitBox(m_Transform->GetWorldPosition(), static_cast<float>(size.x), static_cast<float>(size.y));
	m_Size = textureRenderComponent->GetTexture()->GetSize();

	m_CurrentState = std::make_unique<RockIdleState>();
	m_CurrentState->OnEnter(GetOwner());
}

void RockComponent::Update()
{

	CheckAndTransitionStates(m_CurrentState->Update(GetOwner()));
}

glm::vec2 RockComponent::GetBottomMiddle() const
{
	auto pos = m_Transform->GetWorldPosition();
	pos.x += m_Size.x / 2.f;
	pos.y += m_Size.y;
	return pos;
}

void RockComponent::CheckAndTransitionStates(std::unique_ptr<RockState> newState)
{
	if (newState && typeid(*m_CurrentState) != typeid(*newState))
	{
		m_CurrentState->OnExit(GetOwner());
		m_CurrentState = std::move(newState);
		m_CurrentState->OnEnter(GetOwner());
	}
}

void RockComponent::Notify(const GameEvent& event, Twengine::GameObject*)
{
	if (event.id == make_sdbm_hash("OnEnemyCrushed") && !GetOwner()->IsMarkedForDestruction())
	{
		GameManager::GetInstance().GetScoreComponent()->Notify(GameEvent(make_sdbm_hash("RockCrushedEnemies")), GetOwner());
		GetOwner()->MarkForDestruction();
	}
}
