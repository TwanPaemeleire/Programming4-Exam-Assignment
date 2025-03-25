#include "DisplayLivesComponent.h"
#include "GameObject.h"
#include "TextComponent.h"
#include "HealthComponent.h"

DisplayLivesComponent::DisplayLivesComponent(Twengine::GameObject* owner)
	:Component(owner)
{
}

void DisplayLivesComponent::Start()
{
	m_TextComponent = GetOwner()->GetComponent<Twengine::TextComponent>();
	m_TextComponent->SetText("# Lives: 3");
}

void DisplayLivesComponent::Notify(const GameEvent& event, Twengine::GameObject* observedObject)
{
	if (event.id == make_sdbm_hash("PlayerDied"))
	{
		m_TextComponent->SetText("# Lives: " + std::to_string(observedObject->GetComponent<HealthComponent>()->GetLives()));
	}
}
