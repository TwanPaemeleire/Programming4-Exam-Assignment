#include "GameManager.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "HealthComponent.h"
#include "GridComponent.h"
#include "GroundComponent.h"
#include "DigDugComponent.h"

void GameManager::StartGameFromMenu(GameMode)
{
	Twengine::SceneManager::GetInstance().GetPersistentScene().ActivateAllObjects();
	Twengine::SceneManager::GetInstance().SetCurrentScene("Level1");
}

void GameManager::Notify(const GameEvent& event, Twengine::GameObject* observedObject)
{
	if (event.id == make_sdbm_hash("PlayerDied"))
	{
		LivesComponent* digDugLives = observedObject->GetComponent<LivesComponent>();
		// full death -> go to score
		if (digDugLives->GetLives() <= 0)
		{
			if (m_GameMode == GameMode::SinglePlayer)
			{
				Twengine::SceneManager::GetInstance().SetCurrentScene("HighScoreScreen");
			}
			else
			{
				Twengine::SceneManager::GetInstance().SetCurrentScene("MainMenu");
			}
		}
		// lives left -> reload scene
		else
		{
			m_GridComponent->Reset();
			m_GroundComponent->Reset();
			m_PlayerTransform->GetOwner()->GetComponent<DigDugComponent>()->Reset();
			Twengine::SceneManager::GetInstance().GetCurrentScene().Reload();
		}
	}
}
