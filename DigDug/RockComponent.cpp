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
#include <glm.hpp>

RockComponent::RockComponent(Twengine::GameObject* owner)
	:Component(owner)
{
}

void RockComponent::Start()
{
	Twengine::TextureRenderComponent* textureComponent = GetOwner()->GetComponent<Twengine::TextureRenderComponent>();
	glm::ivec2 size = textureComponent->GetTexture()->GetSize();
	GetOwner()->GetComponent<Twengine::RectColliderComponent>()->SetHitBox(m_Transform->GetWorldPosition(), static_cast<float>(size.x), static_cast<float>(size.y));

	m_CurrentState = std::make_unique<RockIdleState>();
	m_CurrentState->OnEnter(GetOwner());
}

void RockComponent::Update()
{

	CheckAndTransitionStates(m_CurrentState->Update(GetOwner()));
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