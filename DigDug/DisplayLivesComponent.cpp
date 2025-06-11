#include "DisplayLivesComponent.h"
#include "GameObject.h"
#include "TextComponent.h"
#include "LivesComponent.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "GridComponent.h"
#include "GameManager.h"

DisplayLivesComponent::DisplayLivesComponent(Twengine::GameObject* owner)
	:Component(owner)
{
	m_LifeTexture = Twengine::ResourceManager::GetInstance().LoadTexture("DigDug/LivesDisplay.png");
}

void DisplayLivesComponent::Render() const
{
	for (int liveIndex{}; liveIndex < m_LivesLeft; ++liveIndex)
	{
		const auto& pos = GetOwner()->GetTransform()->GetWorldPosition();
		Twengine::Renderer::GetInstance().RenderTexture(*m_LifeTexture, pos.x + m_LifeDrawOffset * liveIndex, pos.y);
	}
}

void DisplayLivesComponent::Notify(const GameEvent& event, Twengine::GameObject* observedObject)
{
	if (event.id == make_sdbm_hash("PlayerDied"))
	{
		m_LivesLeft = observedObject->GetComponent<LivesComponent>()->GetLives();
	}
}
