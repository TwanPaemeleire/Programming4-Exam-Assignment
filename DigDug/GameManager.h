#pragma once
#include "Singleton.h"
#include "Observer.h"

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

	void SetPlayerTransform(Twengine::TransformComponent* playerTransform) { m_PlayerTransform = playerTransform; }
	Twengine::TransformComponent* GetPlayerTransform() const { return m_PlayerTransform; }

	void SetScoreComponent(ScoreComponent* scoreComponent) { m_ScoreComponent = scoreComponent; }
	ScoreComponent* GetScoreComponent() const { return m_ScoreComponent; }

	void SetScoreFileComponent(ScoreFileComponent* scoreFileComponent) { m_ScoreFileComponent = scoreFileComponent; }
	ScoreFileComponent* GetScoreFileComponent() const { return m_ScoreFileComponent; }

	void StartGameFromMenu(GameMode gameMode);
	void Notify(const GameEvent& event, Twengine::GameObject* observedObject) override;

private:
	friend class Singleton<GameManager>;
	GameManager() = default;

	GridComponent* m_GridComponent{};
	GroundComponent* m_GroundComponent{};
	Twengine::TransformComponent* m_PlayerTransform{};
	ScoreComponent* m_ScoreComponent{};
	ScoreFileComponent* m_ScoreFileComponent{};
	GameMode m_GameMode{};

};