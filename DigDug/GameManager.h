#pragma once
#include "Singleton.h"

class GridComponent;

class GameManager final : public Twengine::Singleton<GameManager>
{
public:
	void SetGrid(GridComponent* grid) { m_GridComponent = grid; }
	GridComponent* GetGrid() const { return m_GridComponent; }
private:
	friend class Singleton<GameManager>;
	GameManager() = default;

	GridComponent* m_GridComponent{};
};

