#pragma once
#include "Singleton.h"

namespace Twengine
{
	class TransformComponent;
}

class GridComponent;
class GroundComponent;
class ScoreComponent;

enum class GameMode
{
	SinglePlayer,
	Coop,
	Versus
};

class GameManager final : public Twengine::Singleton<GameManager>
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

	void SetGameMode(GameMode gameMode) { m_GameMode = gameMode; }

private:
	friend class Singleton<GameManager>;
	GameManager() = default;

	GridComponent* m_GridComponent{};
	GroundComponent* m_GroundComponent{};
	Twengine::TransformComponent* m_PlayerTransform{};
	ScoreComponent* m_ScoreComponent{};
	GameMode m_GameMode{};
};