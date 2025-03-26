#include "DigDugComponent.h"
#include "MyTime.h"
#include "GameObject.h"
#include "AnimatedTextureComponent.h"

DigDugComponent::DigDugComponent(Twengine::GameObject* owner)
	:Component(owner)
{

}

void DigDugComponent::Start()
{
	m_AnimationComponent = GetOwner()->GetComponent<Twengine::AnimatedTextureComponent>();
	m_AnimationComponent->AddAnimation("Pooka/PookaMove.png", 2);
	m_AnimationComponent->AddAnimation("Pooka/PookaPump.png", 4);
	m_AnimationComponent->AddAnimation("Pooka/PookaGhost.png", 2);
	m_AnimationComponent->AddAnimation("Pooka/PookaCrushed.png", 1);
	m_AnimationComponent->PlayAnimation("PookaCrushed");
}

void DigDugComponent::Update()
{
	m_Angle += 20 * Twengine::Time::GetInstance().deltaTime;
	m_AnimationComponent->SetRotationAngle(m_Angle);
}