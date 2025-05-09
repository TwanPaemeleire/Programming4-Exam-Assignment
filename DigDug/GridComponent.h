#pragma once
#include "Component.h"
#include "glm.hpp"
#include <utility>
#include <vector>

struct SDL_Rect;

struct Cell
{
	glm::vec2 topLeft{};
	int layer{};
	bool canGoUp{ false };
	bool canGoDown{ false };
	bool canGoLeft{ false };
	bool canGoRight{ false };
};

class GridComponent : public Twengine::Component
{
public:
	GridComponent(Twengine::GameObject* owner);
	virtual ~GridComponent() override = default;
	GridComponent(const GridComponent& other) = delete;
	GridComponent(GridComponent&& other) = delete;
	GridComponent& operator=(const GridComponent& other) = delete;
	GridComponent& operator=(GridComponent&& other) = delete;

	virtual void Render() const override;

	Cell* GetCell(int row, int column) { return &m_Grid[row][column]; }
	Cell* GetCell(std::pair<int, int> index) { return &m_Grid[index.first][index.second]; }
	int GetRows() const { return m_Rows; }
	int GetColumns() const { return m_Columns; }
	float GetCellSize() const { return m_CellSize; }

	std::pair<int, int> GetIndexFromPosition(glm::vec2 pos) const;
	glm::vec2 GetPositionFromIndex(int row, int column) const { return m_Grid[row][column].topLeft; }
	glm::vec2 GetPositionFromIndex(std::pair<int, int> index) const { return m_Grid[index.first][index.second].topLeft; }
	std::vector<Cell*> GetCellsInRect(const SDL_Rect& rect);

private:
	static constexpr int m_Rows{18};
	static constexpr int m_Columns{14};
	Cell m_Grid[m_Rows][m_Columns];

	float m_CellSize{};
};

