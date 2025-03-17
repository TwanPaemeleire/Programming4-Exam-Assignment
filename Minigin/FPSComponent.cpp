#include "FPSComponent.h"
#include "GameObject.h"
#include "TextComponent.h"
#include "Time.h"

#include <sstream>
#include <iomanip> // Needed To Set The Precision

Twengine::FPSComponent::FPSComponent(GameObject* owner)
	:Component(owner)
{
}

void Twengine::FPSComponent::Start()
{
	m_TextComponent = GetOwner()->GetComponent<TextComponent>();
	//m_TextComponent->SetColor(255, 0, 0, 255);
}

void Twengine::FPSComponent::Update()
{
	std::stringstream stream;
	stream << std::fixed << std::setprecision(1) << 1.f / Time::GetInstance().deltaTime <<" FPS";
	m_TextComponent->SetText(stream.str());
}
