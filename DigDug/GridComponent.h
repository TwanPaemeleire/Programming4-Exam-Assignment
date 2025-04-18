#pragma once
#include "Component.h"
#include "glm.hpp"
#include <utility>

struct Cell
{
	glm::vec2 topLeft{};
	int layer{};
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
	int GetRows() const { return m_Rows; }
	int GetColumns() const { return m_Columns; }
	float GetCellSize() const { return m_CellSize; }

	std::pair<int, int> GetIndexFromPosition(glm::vec2 pos) const;
	glm::vec2 GetPositionFromIndex(int row, int column) const { return m_Grid[row][column].topLeft; }

private:
	static constexpr int m_Rows{18};
	static constexpr int m_Columns{14};
	Cell m_Grid[m_Rows][m_Columns];

	float m_CellSize{};
};

