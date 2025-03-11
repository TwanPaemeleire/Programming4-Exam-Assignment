#include "DisplayPointsComponent.h"
#include "GameObject.h"
#include "TextComponent.h"
#include "PointComponent.h"

DisplayPointsComponent::DisplayPointsComponent(GameObject* owner)
	:Component(owner)
{
}

void DisplayPointsComponent::Start()
{
	m_TextComponent = GetOwner()->GetComponent<TextComponent>();
	m_TextComponent->SetText("Score: 0");
}

void DisplayPointsComponent::Notify(GameEvent event, GameObject* observedObject)
{
	if (event == GameEvent::PointsChanged)
	{
		m_TextComponent->SetText("Score: " + std::to_string(observedObject->GetComponent<PointComponent>()->GetPoints()));
	}
}
