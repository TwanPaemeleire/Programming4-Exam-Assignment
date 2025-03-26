#include "GridComponent.h"
#include "Renderer.h"

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
