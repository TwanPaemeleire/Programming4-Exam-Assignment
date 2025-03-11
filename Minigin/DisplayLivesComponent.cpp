#include "DisplayLivesComponent.h"
#include "GameObject.h"
#include "TextComponent.h"
#include "HealthComponent.h"

DisplayLivesComponent::DisplayLivesComponent(GameObject* owner)
	:Component(owner)
{
}

void DisplayLivesComponent::Start()
{
	m_TextComponent = GetOwner()->GetComponent<TextComponent>();
	m_TextComponent->SetText("# Lives: 3");
}

void DisplayLivesComponent::Notify(GameEvent event, GameObject* observedObject)
{
	if (event == GameEvent::PlayerDied)
	{
		m_TextComponent->SetText("# Lives: " + std::to_string(observedObject->GetComponent<HealthComponent>()->GetLives()));
	}
}
