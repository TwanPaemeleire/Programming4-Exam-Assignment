#include "FygarFireComponent.h"
#include "AnimationComponent.h"
#include "RectColliderComponent.h"
#include "GameObject.h"
#include "GroundComponent.h"
#include "GameManager.h"
#include "Renderer.h"
#include "Texture2D.h"

FygarFireComponent::FygarFireComponent(Twengine::GameObject* owner)
	:Component(owner)
{
	GetOwner()->SetTag(make_sdbm_hash("FygarFire"));
}

void FygarFireComponent::Start()
{
	m_AnimationComponent = GetOwner()->AddComponent<Twengine::AnimationComponent>();
	m_AnimationComponent->AddAnimation("Fygar/FygarFire.png", make_sdbm_hash("FygarFire"), 3, 2);
	m_AnimationComponent->PlayAnimation(make_sdbm_hash("FygarFire"), 1.f);
	m_CurrentFrameIndex = m_AnimationComponent->GetCurrentFrameIndex();
	m_FrameWidth = m_AnimationComponent->GetAnimationFrameWidth();
	m_FrameHeight = m_AnimationComponent->GetAnimationFrameHeight();

	m_RectColliderComponent = GetOwner()->AddComponent<Twengine::RectColliderComponent>();
	float hitBoxWidth = m_FrameWidth * ((m_CurrentFrameIndex + 1) / 3.f); // Hitbox depends on frame index, it gets bigger as the animation progresses
	glm::vec2 hitBoxPos = m_Transform->GetWorldPosition();
	if (!m_ShotToRight)
	{
		m_AnimationComponent->SetFlipHorizontal(true);
		hitBoxPos.x -= hitBoxWidth;
		
		glm::vec2 posOnLeft = m_Transform->GetLocalPosition();
		posOnLeft.x -= hitBoxWidth * 3.f;
		m_Transform->SetLocalPosition(posOnLeft);
	}
	else
	{
		glm::vec2 posOnRight = m_Transform->GetLocalPosition();
		posOnRight.x += GetOwner()->GetParent()->GetComponent<Twengine::AnimationComponent>()->GetAnimationFrameWidth();
		m_Transform->SetLocalPosition(posOnRight);
		hitBoxPos.x += GetOwner()->GetParent()->GetComponent<Twengine::AnimationComponent>()->GetAnimationFrameWidth();
	}
	m_RectColliderComponent->ChangeHitBox(hitBoxPos, hitBoxWidth, m_FrameHeight);
}

void FygarFireComponent::Update()
{
	if (m_AnimationComponent->HasFinishedPlayingOnce())
	{
		GetOwner()->MarkForDestruction();
		return;
	}
	int frameIndex = m_AnimationComponent->GetCurrentFrameIndex();
	if (m_CurrentFrameIndex != frameIndex)
	{
		m_CurrentFrameIndex = frameIndex;
		float hitBoxWidth = m_FrameWidth * ((m_CurrentFrameIndex + 1) / 3.f); // Hitbox depends on frame index, it gets bigger as the animation progresses
		if (IsGoingToHitWall(hitBoxWidth))
		{
			GetOwner()->MarkForDestruction();
			return;
		}
		AdjustHitBox(hitBoxWidth);
	}
}

void FygarFireComponent::RenderUI()
{
	//auto pos = m_Transform->GetWorldPosition();
	//Twengine::Renderer::GetInstance().DrawPoint(pos.x, pos.y, SDL_Color(0, 255, 255), 5);
}

void FygarFireComponent::AdjustHitBox(float newWidth)
{
	if (m_ShotToRight)
	{
		m_RectColliderComponent->ChangeHitBox(m_Transform->GetWorldPosition(), newWidth, m_FrameHeight);
	}
	else
	{
		glm::vec2 newPos =	m_RectColliderComponent->GetHitBox()->topLeft;
		newPos.x -= (newWidth - m_RectColliderComponent->GetHitBox()->width);
		m_RectColliderComponent->ChangeHitBox(newPos, newWidth, m_FrameHeight);
	}
}

bool FygarFireComponent::IsGoingToHitWall(float newWidth)
{
	glm::vec2 currentOuterPos = m_RectColliderComponent->GetHitBox()->topLeft;
	currentOuterPos.y += m_RectColliderComponent->GetHitBox()->height / 2;
	glm::vec2 newOuterPos = currentOuterPos;
	float widthChange = newWidth - m_RectColliderComponent->GetHitBox()->width;
	newOuterPos.x += (m_ShotToRight) ? widthChange : -widthChange;

	return !GameManager::GetInstance().GetGround()->CanMoveBetween(currentOuterPos, newOuterPos, 5);
}
