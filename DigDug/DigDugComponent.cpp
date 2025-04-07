#include "DigDugComponent.h"
#include "MyTime.h"
#include "GameObject.h"
#include "AnimationComponent.h"
#include "RectColliderComponent.h"
#include <iostream>
#include "ServiceLocator.h"
#include "SoundSystem.h"

DigDugComponent::DigDugComponent(Twengine::GameObject* owner)
	:Component(owner)
{
	m_AnimationComponent = GetOwner()->AddComponent<Twengine::AnimationComponent>();
	m_RectColliderComponent =  GetOwner()->AddComponent<Twengine::RectColliderComponent>();
	m_RectColliderComponent->GetOnCollisionEvent()->AddObserver(this);
}

void DigDugComponent::Start()
{
	m_AnimationComponent->AddAnimation("DigDug/DigDugMove.png", 2);
	m_AnimationComponent->AddAnimation("DigDug/DigDugDigging.png", 4);
	m_AnimationComponent->AddAnimation("DigDug/DigDugIdle.png", 1);

	m_AnimationComponent->PlayAnimation("DigDugMove");
	glm::vec2 pos = GetOwner()->GetTransform()->GetLocalPosition();
	m_RectColliderComponent->SetHitBox(pos, m_AnimationComponent->GetAnimationFrameWidth(), m_AnimationComponent->GetAnimationFrameHeight());

	Twengine::ServiceLocator::get_sound_system().RequestPlayMusic("Level/LevelMusic.wav", 0.9f);
}

void DigDugComponent::Update()
{
	//m_Angle += 20 * Twengine::Time::GetInstance().deltaTime;
	//m_AnimationComponent->SetRotationAngle(m_Angle);
}

void DigDugComponent::Notify(const GameEvent& event, Twengine::GameObject* observedObject)
{
	if (event.id == make_sdbm_hash("OnCollision") && observedObject->GetTag() == make_sdbm_hash("CollisionTest"))
	{
		std::cout << "COLLISION DETECTED IN PLAYER COMPONENT" << std::endl;
		Twengine::ServiceLocator::get_sound_system().RequestPlaySound("DigDug/DigDugShot.wav", 0.9f);
		observedObject;
	}
}
