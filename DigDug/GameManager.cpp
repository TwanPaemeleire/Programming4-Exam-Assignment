#include "GameManager.h"
#include "SceneManager.h"
#include "GameObject.h"
#include "LivesComponent.h"
#include "GridComponent.h"
#include "GroundComponent.h"
#include "DigDugComponent.h"
#include "InputManager.h"
#include "ScoreFileComponent.h"
#include <iostream>

Twengine::TransformComponent* GameManager::GetClosestPlayerTransform(glm::vec2 pos) const
{
	if (GameManager::GetInstance().CurrentGameMode() != GameMode::Coop) return m_PlayerTransforms[0];
	else
	{
		const float player1Dist = glm::distance(pos, glm::vec2(m_PlayerTransforms[0]->GetWorldPosition()));
		const float player2Dist = glm::distance(pos, glm::vec2(m_PlayerTransforms[1]->GetWorldPosition()));

		return (player1Dist < player2Dist) ? m_PlayerTransforms[0] : m_PlayerTransforms[1];
	}
}

void GameManager::StartGameFromMenu(GameMode gameMode)
{
	m_GameMode = gameMode;
	Twengine::InputManager::GetInstance().ClearCommandMap(make_sdbm_hash("MainMenu"));
	Twengine::InputManager::GetInstance().ClearCommandMap(make_sdbm_hash("Game"));
	Twengine::SceneManager::GetInstance().GetPersistentScene().Reload();
	Twengine::SceneManager::GetInstance().RequestSetCurrentScene("Level1");
}

void GameManager::Notify(const GameEvent& event, Twengine::GameObject* observedObject)
{
	if (event.id == make_sdbm_hash("PlayerDied"))
	{
		LivesComponent* digDugLives = observedObject->GetComponent<LivesComponent>();
		//Player has no more lives left, so go to the scene for saving the score
		if (digDugLives->GetLives() <= 0)
		{
			Twengine::InputManager::GetInstance().ClearCommandMap(make_sdbm_hash("Game"));
			Twengine::SceneManager::GetInstance().GetPersistentScene().DeactivateAllObjects();
			if (m_GameMode == GameMode::SinglePlayer)
			{
				Twengine::SceneManager::GetInstance().RequestSetCurrentScene("HighScoreScene");
			}
			else
			{
				Twengine::SceneManager::GetInstance().RequestSetCurrentScene("MainMenu");
			}
		}
		// Player has lives left, so reload the scene
		else
		{
			m_GridComponent->Reset();
			m_GroundComponent->Reset();
			for (const Twengine::TransformComponent* playerTransform : m_PlayerTransforms)
			{
				playerTransform->GetOwner()->GetComponent<DigDugComponent>()->Reset();
			}
			Twengine::SceneManager::GetInstance().GetCurrentScene().Reload();
		}
	}
	else if (event.id == make_sdbm_hash("GameFinished"))
	{
		if (m_GameMode == GameMode::SinglePlayer)
		{
			m_ScoreFileComponent->WriteHighScores();
		}
		Twengine::InputManager::GetInstance().ClearCommandMap(make_sdbm_hash("Game"));
		Twengine::InputManager::GetInstance().ClearCommandMap(make_sdbm_hash("HighScoreScene"));
		Twengine::SceneManager::GetInstance().GetPersistentScene().Reload();
		Twengine::SceneManager::GetInstance().GetPersistentScene().DeactivateAllObjects();
		Twengine::SceneManager::GetInstance().RequestSetCurrentScene("MainMenu");
	}
	if (event.id == make_sdbm_hash("OnEnemyKilled"))
	{
		--m_AmountOfEnemiesAlive;
		if (m_AmountOfEnemiesAlive <= 0)
		{
			GoToNextLevel();
		}
	}
	if (event.id == make_sdbm_hash("GoToNextLevel"))
	{
		GoToNextLevel();
	}
}

void GameManager::GoToNextLevel()
{
	const unsigned int currentSceneId = Twengine::SceneManager::GetInstance().GetCurrentScene().GetId();
	// Current level was final level, so game is done
	if (currentSceneId >= Twengine::Scene::s_IdCounter - 2) // -2 Because persistent scene isn't in scenes vec and we want this to trigger when we want to go to the highscore scene
	{
		Twengine::InputManager::GetInstance().ClearCommandMap(make_sdbm_hash("Game"));
		Twengine::SceneManager::GetInstance().GetPersistentScene().DeactivateAllObjects();
		if (m_GameMode == GameMode::SinglePlayer)
		{
			Twengine::SceneManager::GetInstance().RequestSetCurrentScene("HighScoreScene");
		}
		else
		{
			Twengine::SceneManager::GetInstance().RequestSetCurrentScene("MainMenu");
		}
	}
	else // Go to the next level
	{
		m_GridComponent->Reset();
		m_GroundComponent->Reset();
		for (const Twengine::TransformComponent* playerTransform : m_PlayerTransforms)
		{
			playerTransform->GetOwner()->GetComponent<DigDugComponent>()->Reset();
		}
		Twengine::SceneManager::GetInstance().RequestSetCurrentScene(currentSceneId);
	}
}
