#include "GridComponent.h"
#include "Renderer.h"
#include <algorithm>
#include "SDL_rect.h"

GridComponent::GridComponent(Twengine::GameObject* owner)
	:Component(owner)
{
	m_CellSize = static_cast<float>(Twengine::Renderer::GetInstance().GetWindowWidth()) / m_Columns;
	// Create The Grid
	for (int rowIndex{}; rowIndex < m_Rows; ++rowIndex)
	{
		for (int columnIndex{}; columnIndex < m_Columns; ++columnIndex)
		{
			m_Grid[rowIndex][columnIndex].topLeft = glm::vec2(m_CellSize * columnIndex, m_CellSize * rowIndex);
		}
	}
}

void GridComponent::Render() const
{
	for (int rowIndex{}; rowIndex < m_Rows; ++rowIndex)
	{
		for (int columnIndex{}; columnIndex < m_Columns; ++columnIndex)
		{
			glm::vec2 pos = m_Grid[rowIndex][columnIndex].topLeft;
			Twengine::Renderer::GetInstance().DrawRectangle(pos.x, pos.y, m_CellSize, m_CellSize, SDL_Color(255, 0, 0, 255));
		}
	}
}

std::pair<int, int> GridComponent::GetIndexFromPosition(glm::vec2 pos) const
{
	int row = static_cast<int>(std::floor(pos.y / m_CellSize));
	int column = static_cast<int>(std::floor(pos.x / m_CellSize));

	column = std::clamp(column, 0, m_Columns - 1);
	row = std::clamp(row, 0, m_Rows - 1);

	return std::pair<int, int>(row, column);
}

std::pair<int, int> GridComponent::GetIndexFromCell(Cell* cell) const
{
	cell;
	return std::pair<int, int>();
}

std::vector<Cell*> GridComponent::GetCellsInRect(const SDL_Rect& rect)
{
	glm::vec2 topLeft = glm::vec2(rect.x, rect.y);
	glm::vec2 bottomRight = glm::vec2(rect.x + rect.w, rect.y + rect.h);

	auto [minRow, minCol] = GetIndexFromPosition(topLeft);
	auto [maxRow, maxCol] = GetIndexFromPosition(bottomRight);

	std::vector<Cell*> cellsInRect;
	cellsInRect.reserve((maxRow - minRow) * (maxCol - minCol));

	for (int rowIdx = minRow; rowIdx <= maxRow; ++rowIdx)
	{
		for (int columnIdx = minCol; columnIdx <= maxCol; ++columnIdx)
		{
			cellsInRect.push_back(GetCell(rowIdx, columnIdx));
		}
	}
	return cellsInRect;
}
