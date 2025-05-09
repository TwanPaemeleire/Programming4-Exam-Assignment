#include "FPSComponent.h"
#include "GameObject.h"
#include "TextComponent.h"
#include "MyTime.h"

#include <sstream>
#include <iomanip> // Needed to set the precision

Twengine::FPSComponent::FPSComponent(GameObject* owner)
	:Component(owner)
{
}

void Twengine::FPSComponent::Start()
{
	m_TextComponent = GetOwner()->GetComponent<TextComponent>();
}

void Twengine::FPSComponent::Update()
{
	std::stringstream stream;
	stream << std::fixed << std::setprecision(1) << 1.f / Time::GetInstance().deltaTime <<" FPS";
	m_TextComponent->SetText(stream.str());
}
