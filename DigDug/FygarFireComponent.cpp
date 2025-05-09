#include "FygarFireComponent.h"
#include "AnimationComponent.h"
#include "RectColliderComponent.h"
#include "GameObject.h"

FygarFireComponent::FygarFireComponent(Twengine::GameObject* owner)
	:Component(owner)
{
	GetOwner()->SetTag(make_sdbm_hash("FygarFire"));
}

void FygarFireComponent::Start()
{
	m_AnimationComponent = GetOwner()->AddComponent<Twengine::AnimationComponent>();
	m_AnimationComponent->AddAnimation("Fygar/FygarFire.png", make_sdbm_hash("FygarFire"), 3);
	m_AnimationComponent->PlayAnimation(make_sdbm_hash("FygarFire"), 1.f);
	m_CurrentFrameIndex = m_AnimationComponent->GetCurrentFrameIndex();
	m_FrameWidth = m_AnimationComponent->GetAnimationFrameWidth();
	m_FrameHeight = m_AnimationComponent->GetAnimationFrameHeight();

	m_RectColliderComponent = GetOwner()->AddComponent<Twengine::RectColliderComponent>();
	float hitBoxWidth = m_FrameWidth * ((m_CurrentFrameIndex + 1) / 3.f); // Hitbox depends on frame index, it gets bigger as the animation progresses
	m_RectColliderComponent->SetHitBox(m_Transform->GetWorldPosition(), hitBoxWidth, m_FrameHeight);
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
		m_RectColliderComponent->SetHitBox(m_Transform->GetWorldPosition(), hitBoxWidth, m_FrameHeight);
	}
}