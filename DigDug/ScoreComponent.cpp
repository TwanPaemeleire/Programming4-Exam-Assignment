#include "ScoreComponent.h"
#include "Event.h"
#include "GameObject.h"
#include "PookaComponent.h"
#include "FygarComponent.h"
#include "GameManager.h"
#include "GridComponent.h"
#include "TransformComponent.h"

ScoreComponent::ScoreComponent(Twengine::GameObject* owner)
	:Twengine::Component(owner)
{
	m_ScoreChangedEvent = std::make_unique<Twengine::Event>();
}

void ScoreComponent::SetScore(int points)
{
	m_Score = points;
	m_ScoreChangedEvent->NotifyObservers(GameEvent(make_sdbm_hash("ScoreChanged")), GetOwner());
}

void ScoreComponent::IncreaseScore(int scoreIncrease)
{
	m_Score += scoreIncrease;
	m_ScoreChangedEvent->NotifyObservers(GameEvent(make_sdbm_hash("ScoreChanged")), GetOwner());
}

void ScoreComponent::Notify(const GameEvent& event, Twengine::GameObject* eventSender)
{
	if (event.id == make_sdbm_hash("PlayerDied"))
	{
		m_Score -= 200;
		m_ScoreChangedEvent->NotifyObservers(GameEvent(make_sdbm_hash("ScoreChanged")), GetOwner());
	}
	else if (event.id == make_sdbm_hash("OnEnemyKilled"))
	{
		int scoreIncrease = 0;
		int layer = GetEnemyLayer(eventSender->GetTransform()->GetWorldPosition());
		int increasePerLayer = 0;
		int scoreMultiplier = 1;
		if (eventSender->GetComponent<PookaComponent>())
		{
			scoreIncrease = 200;
			increasePerLayer = 100;
		}
		else if (eventSender->GetComponent<FygarComponent>())
		{
			scoreIncrease = 400;
			increasePerLayer = 200;
			auto fygarPos = eventSender->GetTransform()->GetWorldPosition();
			int playerRow = GameManager::GetInstance().GetGrid()->GetIndexFromPosition(GameManager::GetInstance().GetClosestPlayerTransform(fygarPos)->GetWorldPosition()).first;
			int fygarRow = GameManager::GetInstance().GetGrid()->GetIndexFromPosition(fygarPos).first;
			if (playerRow == fygarRow) scoreMultiplier = 2;
		}
		int adjustedLayer = std::max(0, layer - 1);
		scoreIncrease += (increasePerLayer * adjustedLayer);
		scoreIncrease *= scoreMultiplier;
		IncreaseScore(scoreIncrease);
	}
	else if (event.id == make_sdbm_hash("Pickup"))
	{
		m_Score += 50;
		m_ScoreChangedEvent->NotifyObservers(GameEvent(make_sdbm_hash("ScoreChanged")), GetOwner());
	}
	else if (event.id == make_sdbm_hash("RockCrushedEnemies"))
	{
		int scoreIncrease = 0;
		size_t amountOfEnemies = eventSender->GetChildCount();

		switch (amountOfEnemies)
		{
		case 1: scoreIncrease = 1000; break;
		case 2: scoreIncrease = 2500; break;
		case 3: scoreIncrease = 4000; break;
		case 4: scoreIncrease = 6000; break;
		case 5: scoreIncrease = 8000; break;
		case 6: scoreIncrease = 10000; break;
		case 7: scoreIncrease = 12000; break;
		case 8: scoreIncrease = 15000; break;
		default: scoreIncrease = 0; break;
		}
		IncreaseScore(scoreIncrease);
	}
}

int ScoreComponent::GetEnemyLayer(const glm::vec2& pos) const
{
	return GameManager::GetInstance().GetGrid()->GetCell(GameManager::GetInstance().GetGrid()->GetIndexFromPosition(pos))->layer;
}
