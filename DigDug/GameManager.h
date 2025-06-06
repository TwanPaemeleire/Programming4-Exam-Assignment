#pragma once
#include "Singleton.h"
#include "Observer.h"
#include <glm.hpp>
#include <vector>

namespace Twengine
{
	class TransformComponent;
	class GameObject;
}

class GridComponent;
class GroundComponent;
class ScoreComponent;
class ScoreFileComponent;

enum class GameMode
{
	SinglePlayer,
	Coop,
	Versus
};

class GameManager final : public Twengine::Singleton<GameManager>, public Twengine::Observer
{
public:
	void SetGrid(GridComponent* grid) { m_GridComponent = grid; }
	GridComponent* GetGrid() const { return m_GridComponent; }

	void SetGround(GroundComponent* ground) { m_GroundComponent = ground; }
	GroundComponent* GetGround() const { return m_GroundComponent; }

	std::vector<Twengine::TransformComponent*> GetPlayerTransforms() const { return m_PlayerTransforms; }
	Twengine::TransformComponent* GetClosestPlayerTransform(glm::vec2 pos) const;
	void AddPlayerTransform(Twengine::TransformComponent* playerTransform) { m_PlayerTransforms.emplace_back(playerTransform); }
	void ClearPlayerTransforms() { m_PlayerTransforms.clear(); }

	void SetScoreComponent(ScoreComponent* scoreComponent) { m_ScoreComponent = scoreComponent; }
	ScoreComponent* GetScoreComponent() const { return m_ScoreComponent; }

	void SetScoreFileComponent(ScoreFileComponent* scoreFileComponent) { m_ScoreFileComponent = scoreFileComponent; }
	ScoreFileComponent* GetScoreFileComponent() const { return m_ScoreFileComponent; }

	void StartGameFromMenu(GameMode gameMode);
	void Notify(const GameEvent& event, Twengine::GameObject* observedObject) override;

	void IncreaseEnemyCount(){++m_AmountOfEnemiesAlive;}
	void ResetEnemyCount() { m_AmountOfEnemiesAlive = 0; }
	int AmountOfEnemiesAlive() const { return m_AmountOfEnemiesAlive; }

	GameMode CurrentGameMode() const { return m_GameMode; }

private:
	friend class Singleton<GameManager>;
	GameManager() = default;

	GridComponent* m_GridComponent{};
	GroundComponent* m_GroundComponent{};
	ScoreComponent* m_ScoreComponent{};
	ScoreFileComponent* m_ScoreFileComponent{};
	GameMode m_GameMode{};

	std::vector<Twengine::TransformComponent*> m_PlayerTransforms{};
	int m_AmountOfEnemiesAlive{0};
};