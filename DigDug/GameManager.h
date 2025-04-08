#pragma once
#include "Singleton.h"

class GridComponent;
class GroundComponent;

class GameManager final : public Twengine::Singleton<GameManager>
{
public:
	void SetGrid(GridComponent* grid) { m_GridComponent = grid; }
	GridComponent* GetGrid() const { return m_GridComponent; }

	void SetGround(GroundComponent* ground) { m_GroundComponent = ground; }
	GroundComponent* GetGround() const { return m_GroundComponent; }
private:
	friend class Singleton<GameManager>;
	GameManager() = default;

	GridComponent* m_GridComponent{};
	GroundComponent* m_GroundComponent{};
};

