#include "DigDugComponent.h"
#include "MyTime.h"
#include "GameObject.h"
#include "AnimatedTextureComponent.h"

DigDugComponent::DigDugComponent(Twengine::GameObject* owner)
	:Component(owner)
{
}

void DigDugComponent::Update()
{
	m_FlipCounter += Twengine::Time::GetInstance().deltaTime;
	if (m_FlipCounter > m_FlipDelay)
	{
		m_FlipCounter -= m_FlipDelay;
		m_FlipHorizontal = !m_FlipHorizontal;
		GetOwner()->GetComponent<Twengine::AnimatedTextureComponent>()->SetFlipHorizontal(m_FlipHorizontal);
	}
}