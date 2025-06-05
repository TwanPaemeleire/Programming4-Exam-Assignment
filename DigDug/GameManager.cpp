#include "GameManager.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "LivesComponent.h"
#include "GridComponent.h"
#include "GroundComponent.h"
#include "DigDugComponent.h"
#include "InputManager.h"
#include "ScoreFileComponent.h"

void GameManager::StartGameFromMenu(GameMode)
{
	Twengine::InputManager::GetInstance().ClearCommandMap(make_sdbm_hash("MainMenu"));
	Twengine::SceneManager::GetInstance().GetPersistentScene().ActivateAllObjects();
	Twengine::SceneManager::GetInstance().SetCurrentScene("Level1");
}

void GameManager::Notify(const GameEvent& event, Twengine::GameObject* /*observedObject*/)
{
	if (event.id == make_sdbm_hash("PlayerDied"))
	{
		//LivesComponent* digDugLives = observedObject->GetComponent<LivesComponent>();
		// Player has no more lives left, so go to the scene for saving the score
		if (true/*digDugLives->GetLives() <= 0*/)
		{
			if (m_GameMode == GameMode::SinglePlayer)
			{
				Twengine::SceneManager::GetInstance().GetPersistentScene().DeactivateAllObjects();
				Twengine::InputManager::GetInstance().ClearCommandMap(make_sdbm_hash("Game"));
				Twengine::SceneManager::GetInstance().SetCurrentScene("HighScoreScene");
			}
			else
			{
				Twengine::SceneManager::GetInstance().SetCurrentScene("MainMenu");
			}
		}
		// Player has lives left, so reload the scene
		else
		{
			m_GridComponent->Reset();
			m_GroundComponent->Reset();
			m_PlayerTransform->GetOwner()->GetComponent<DigDugComponent>()->Reset();
			Twengine::SceneManager::GetInstance().GetCurrentScene().Reload();
		}
	}
	else if (event.id == make_sdbm_hash("GameFinished"))
	{
		if (m_GameMode == GameMode::SinglePlayer)
		{
			m_ScoreFileComponent->WriteHighScores();
		}
		Twengine::SceneManager::GetInstance().GetPersistentScene().Reload();
		Twengine::SceneManager::GetInstance().GetPersistentScene().DeactivateAllObjects();
		Twengine::InputManager::GetInstance().ClearCommandMap(make_sdbm_hash("Game"));
		Twengine::InputManager::GetInstance().ClearCommandMap(make_sdbm_hash("HighScoreScene"));
		Twengine::SceneManager::GetInstance().SetCurrentScene("MainMenu");
	}
}
