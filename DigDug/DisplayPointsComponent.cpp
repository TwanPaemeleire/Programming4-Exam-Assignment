#include "DisplayPointsComponent.h"
#include "GameObject.h"
#include "TextComponent.h"
#include "ScoreComponent.h"

DisplayPointsComponent::DisplayPointsComponent(Twengine::GameObject* owner)
	:Component(owner)
{
}

void DisplayPointsComponent::Start()
{
	m_TextComponent = GetOwner()->GetComponent<Twengine::TextComponent>();
	m_TextComponent->SetText("Score: 0");
}

void DisplayPointsComponent::Notify(const GameEvent& event, Twengine::GameObject* observedObject)
{
	if (event.id == make_sdbm_hash("ScoreChanged"))
	{
		m_TextComponent->SetText("Score: " + std::to_string(observedObject->GetComponent<ScoreComponent>()->GetScore()));
	}
}
