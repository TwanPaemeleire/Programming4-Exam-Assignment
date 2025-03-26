#include "DisplayLivesComponent.h"
#include "GameObject.h"
#include "TextComponent.h"
#include "HealthComponent.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "GridComponent.h"

DisplayLivesComponent::DisplayLivesComponent(Twengine::GameObject* owner)
	:Component(owner)
{
	m_LifeTexture = Twengine::ResourceManager::GetInstance().LoadTexture("DigDug/LivesDisplay.png");
}

void DisplayLivesComponent::Render() const
{
	for (int liveIndex{}; liveIndex < m_LivesLeft; ++liveIndex)
	{
		auto& pos = GetOwner()->GetTransform()->GetWorldPosition();
		Twengine::Renderer::GetInstance().RenderTexture(*m_LifeTexture, pos.x + m_LifeDrawOffset * liveIndex, pos.y);
	}
}

void DisplayLivesComponent::Notify(const GameEvent& event, Twengine::GameObject* observedObject)
{
	if (event.id == make_sdbm_hash("PlayerDied"))
	{
		m_LivesLeft = observedObject->GetComponent<HealthComponent>()->GetLives();
	}
}

void DisplayLivesComponent::Initialize(GridComponent* gridComp)
{
	auto& pos = gridComp->GetCell(gridComp->GetRows() - 1, 0).topLeft;
	GetOwner()->GetTransform()->SetLocalPosition(pos.x, pos.y);
	m_LifeDrawOffset = gridComp->GetCellSize();
}
