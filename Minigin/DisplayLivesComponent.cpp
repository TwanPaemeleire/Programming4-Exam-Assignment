#include "DisplayLivesComponent.h"
#include "GameObject.h"
#include "TextComponent.h"

DisplayLivesComponent::DisplayLivesComponent(GameObject* owner)
	:Component(owner)
{
}

void DisplayLivesComponent::Start()
{
	m_TextComponent = GetOwner()->GetComponent<TextComponent>();
}

void DisplayLivesComponent::OnPlayerDeath()
{
	m_TextComponent->SetText("# Lives: ");
}
