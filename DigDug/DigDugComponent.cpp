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
	m_AnimationComponent->AddAnimation("DigDug/DigDugMove.png", 2);
	m_AnimationComponent->AddAnimation("DigDug/DigDugDigging.png", 4);
	m_AnimationComponent->AddAnimation("DigDug/DigDugIdle.png", 1);

	m_AnimationComponent->PlayAnimation("DigDugMove");
}

void DigDugComponent::Update()
{
	//m_Angle += 20 * Twengine::Time::GetInstance().deltaTime;
	//m_AnimationComponent->SetRotationAngle(m_Angle);
}