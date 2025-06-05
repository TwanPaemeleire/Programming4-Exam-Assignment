#include "GroundComponent.h"
#include "GameManager.h"
#include "GameObject.h"
#include "GridComponent.h"
#include <iostream>
#include "Renderer.h"
#include <queue>
#include <unordered_set>

GroundComponent::GroundComponent(Twengine::GameObject* owner)
	: Component(owner)
{
	m_GridComponent = GameManager::GetInstance().GetGrid();
	m_GridCellSize = m_GridComponent->GetCellSize();
	m_HalfGridCellSize = m_GridCellSize / 2.f;

	m_Transform->SetLocalPosition(m_GridComponent->GetPositionFromIndex(2, 0));
	CreateSurface();
	m_TransparentValue = SDL_MapRGBA(m_Surface->format, 0, 0, 0, 0);
}

void GroundComponent::Render() const
{
	auto& pos = m_Transform->GetWorldPosition();

	SDL_Rect dst;
	SDL_QueryTexture(m_Texture, nullptr, nullptr, &dst.w, &dst.h);

	SDL_Rect dstRect{};
	dstRect.x = static_cast<int>(pos.x);
	dstRect.y = static_cast<int>(pos.y);
	dstRect.w = static_cast<int>(dst.w);
	dstRect.h = static_cast<int>(dst.h);
	SDL_RenderCopy(Twengine::Renderer::GetInstance().GetSDLRenderer(), m_Texture, nullptr, &dstRect);
}

void GroundComponent::RenderUI()
{
	//std::unordered_map<Cell*, Cell*> cellTree = BuildReachableCellTree(glm::vec2(320.f, 128.f));
	//
	//for (const auto& [child, parent] : cellTree)
	//{
	//	Twengine::Renderer::GetInstance().DrawRectangle(child->topLeft.x, child->topLeft.y, m_GridCellSize, m_GridCellSize, SDL_Color(0, 255, 0, 255));
	//}
}

void GroundComponent::ErasePlayerTrail(SDL_Rect playerRect, bool isInWorldSpace)
{
	SDL_Rect rectRelativeToPos = playerRect;
	if (isInWorldSpace)
	{
		// Calculate position relative to where the ground starts, as this won't be the same as player world position
		rectRelativeToPos.x -= static_cast<int>(m_Transform->GetWorldPosition().x);
		rectRelativeToPos.y -= static_cast<int>(m_Transform->GetWorldPosition().y);
	}

	Uint32* pixels = (Uint32*)m_Surface->pixels;

	// Gives the number of bytes in a single row of pixel data on this surface
	int pitchInBytes = m_Surface->pitch;
	// / 4 Because each pixel is 4 bytes and we want to work in the 1D "pixels" array, so this way we get the number of pixels per row, not number of bytes
	int pitch = pitchInBytes / 4;

	// Set all of the calculated rect's pixels to transParent
	for (int y = rectRelativeToPos.y; y < rectRelativeToPos.y + rectRelativeToPos.h; y++)
	{
		for (int x = rectRelativeToPos.x; x < rectRelativeToPos.x + rectRelativeToPos.w; x++)
		{
			// Make sure we don't go out of bounds
			if (x >= 0 && x < m_Surface->w && y >= 0 && y < m_Surface->h)
			{
				pixels[y * pitch + x] = m_TransparentValue;
			}
		}
	}
	
	// Recreate texture from updated surface
	SDL_DestroyTexture(m_Texture);
	m_Texture = SDL_CreateTextureFromSurface(Twengine::Renderer::GetInstance().GetSDLRenderer(), m_Surface);
	SDL_SetTextureBlendMode(m_Texture, SDL_BLENDMODE_BLEND);

	// Calculate back to world space
	rectRelativeToPos.x += static_cast<int>(m_Transform->GetWorldPosition().x);
	rectRelativeToPos.y += static_cast<int>(m_Transform->GetWorldPosition().y);
	std::vector<Cell*> affectedCells = m_GridComponent->GetCellsInRect(rectRelativeToPos);
	for (Cell* cell : affectedCells)
	{
		glm::vec2 cellCenter = glm::vec2(cell->topLeft.x + m_HalfGridCellSize, cell->topLeft.y + m_HalfGridCellSize);

		glm::vec2 upToCheck = glm::vec2(cell->topLeft.x, cell->topLeft.y - (m_GridCellSize * 0.5f));
		glm::vec2 downToCheck = glm::vec2(cell->topLeft.x , cell->topLeft.y + (m_GridCellSize * 1.5f));
		glm::vec2 leftToCheck = glm::vec2(cell->topLeft.x - (m_GridCellSize * 0.5f), cell->topLeft.y);
		glm::vec2 rightToCheck = glm::vec2(cell->topLeft.x + (m_GridCellSize * 1.5f), cell->topLeft.y);
		
		cell->canGoUp = CanMoveBetween(cellCenter, upToCheck);
		cell->canGoDown = CanMoveBetween(cellCenter, downToCheck);
		cell->canGoLeft = CanMoveBetween(cellCenter, leftToCheck);
		cell->canGoRight = CanMoveBetween(cellCenter, rightToCheck);
		UpdateConnectionsWithNeighbors(cell);
	}
}

void GroundComponent::ErasePlayerTrail(int centerX, int centerY, int width, int height, bool isInWorldSpace)
{
	if (isInWorldSpace)
	{
		// Calculate position relative to where the ground starts, as this won't be the same as player world position
		centerX -= static_cast<int>(m_Transform->GetWorldPosition().x);
		centerY -= static_cast<int>(m_Transform->GetWorldPosition().y);
	}

	Uint32* pixels = (Uint32*)m_Surface->pixels;

	// Gives the number of bytes in a single row of pixel data on this surface
	int pitchInBytes = m_Surface->pitch;
	// / 4 Because each pixel is 4 bytes and we want to work in the 1D "pixels" array, so this way we get the number of pixels per row, not number of bytes
	int pitch = pitchInBytes / 4;

	// Calculate bounding box of the ellipse
	int startX = centerX - width / 2;
	int startY = centerY - height / 2;
	int endX = centerX + width / 2;
	int endY = centerY + height / 2;

	float radiusX = width / 2.0f;
	float radiusY = height / 2.0f;

	for (int y = startY; y <= endY; ++y)
	{
		for (int x = startX; x <= endX; ++x)
		{
			// Make sure we don't go out of bounds
			if (x >= 0 && x < m_Surface->w && y >= 0 && y < m_Surface->h)
			{
				float dx = (x - centerX) / radiusX;
				float dy = (y - centerY) / radiusY;
				// Check if pixel is inside the ellipse
				if ((dx * dx + dy * dy) <= 1.0f)
				{
					pixels[y * pitch + x] = m_TransparentValue;
				}
			}
		}
	}

	// Recreate texture from updated surface
	SDL_DestroyTexture(m_Texture);
	m_Texture = SDL_CreateTextureFromSurface(Twengine::Renderer::GetInstance().GetSDLRenderer(), m_Surface);
	SDL_SetTextureBlendMode(m_Texture, SDL_BLENDMODE_BLEND);
}

bool GroundComponent::PositionIsDugOut(const glm::vec2& pos)
{
	// Calculate position relative to where the ground starts, as this won't be the same as player world position
	glm::vec2 relativePos = pos - glm::vec2(m_Transform->GetWorldPosition());
	int x = static_cast<int>(relativePos.x);
	int y = static_cast<int>(relativePos.y);

	// Make sure we don't go out of bounds
	if (x < 0 || x >= m_Surface->w || y < 0 || y >= m_Surface->h)
		return false;

	// Gives the number of bytes in a single row of pixel data on this surface
	int pitchInBytes = m_Surface->pitch;
	// / 4 Because each pixel is 4 bytes and we want to work in the 1D "pixels" array, so this way we get the number of pixels per row, not number of bytes
	int pitch = pitchInBytes / 4;

	Uint32* pixels = (Uint32*)m_Surface->pixels;
	return pixels[y * pitch + x] == m_TransparentValue;
}

glm::vec2 GroundComponent::GetCellTargetToGetCloserToPlayer(const glm::vec2& enemyPos) const
{
	glm::vec2 playerPos = GameManager::GetInstance().GetClosestPlayerTransform(enemyPos)->GetWorldPosition();

	glm::vec2 right = glm::vec2(enemyPos.x + m_GridCellSize, enemyPos.y);
	glm::vec2 down = glm::vec2(enemyPos.x, enemyPos.y + m_GridCellSize);
	glm::vec2 left = glm::vec2(enemyPos.x - m_GridCellSize, enemyPos.y);
	glm::vec2 up = glm::vec2(enemyPos.x, enemyPos.y - m_GridCellSize);

	glm::vec2 rightToCheck = glm::vec2{right.x + (m_GridCellSize * 0.5f), right.y};
	glm::vec2 downToCheck = glm::vec2{down.x, down.y  + (m_GridCellSize * 0.5f)};
	glm::vec2 leftToCheck = glm::vec2{left.x + (m_GridCellSize * 0.5f), left.y};
	glm::vec2 upToCheck = glm::vec2{up.x, up.y + (m_GridCellSize * 0.5f)};

	std::vector<std::pair<glm::vec2, bool>> directions = {
		{ right, CanMoveBetween(enemyPos, rightToCheck) },
		{ down, CanMoveBetween(enemyPos, downToCheck) },
		{ left, CanMoveBetween(enemyPos, leftToCheck) },
		{ up, CanMoveBetween(enemyPos, upToCheck) }
	};

	glm::vec2 bestTarget = enemyPos;
	float shortestDistance = std::numeric_limits<float>::max();

	for (const auto& dir: directions)
	{
		if (dir.second)
		{
			float dist = glm::distance(playerPos, dir.first);
			if (dist < shortestDistance)
			{
				shortestDistance = dist;
				bestTarget = dir.first;
			}
		}
	}

	return bestTarget;
}

bool GroundComponent::CanMoveBetween(const glm::vec2& startPos, const glm::vec2& targetPos, int dirtLeeway) const
{
	int dirtCount = 0;
	const int pitch = m_Surface->pitch / 4;
	Uint32* pixels = (Uint32*)m_Surface->pixels;

	// Convert from world space to ensure calculation is correct
	glm::vec2 localStart = startPos - glm::vec2(m_Transform->GetWorldPosition());
	glm::vec2 localTarget = targetPos - glm::vec2(m_Transform->GetWorldPosition());

	// Calculate the bounding box
	int minX = static_cast<int>(std::min(localStart.x, localTarget.x));
	int maxX = static_cast<int>(std::max(localStart.x, localTarget.x));
	int minY = static_cast<int>(std::min(localStart.y, localTarget.y));
	int maxY = static_cast<int>(std::max(localStart.y, localTarget.y));

	for (int y = minY; y <= maxY; ++y)
	{
		for (int x = minX; x <= maxX; ++x)
		{
			// Out of bounds check
			if (x < 0 || x >= m_Surface->w || y < 0 || y >= m_Surface->h) continue;

			if (pixels[y * pitch + x] != m_TransparentValue) // Pixel has not been dug out
			{
				++dirtCount;
				if (dirtCount > dirtLeeway) return false;
			}
		}
	}

	return true;
}

bool GroundComponent::EnemyCanReachPlayer(const glm::vec2& enemyPos) const
{
	glm::vec2 playerPos = GameManager::GetInstance().GetClosestPlayerTransform(enemyPos)->GetWorldPosition();
	Cell* playerCell = m_GridComponent->GetCell(m_GridComponent->GetIndexFromPosition(playerPos));
	std::unordered_map<Cell*, Cell*> reachableCellTree = BuildReachableCellTree(enemyPos);
	return reachableCellTree.find(playerCell) != reachableCellTree.end();
}

void GroundComponent::Reset()
{
	CreateSurface();
}

void GroundComponent::CreateSurface()
{
	m_Surface = IMG_Load("Level/FullLevel.png");
	// Make sure the blend mode is set correctly so we can "erase" parts of the texture
	SDL_SetSurfaceBlendMode(m_Surface, SDL_BLENDMODE_BLEND);
	m_Texture = SDL_CreateTextureFromSurface(Twengine::Renderer::GetInstance().GetSDLRenderer(), m_Surface);
	SDL_SetTextureBlendMode(m_Texture, SDL_BLENDMODE_BLEND);
}

std::unordered_map<Cell*, Cell*> GroundComponent::BuildReachableCellTree(const glm::vec2& enemyPos) const
{
	std::pair<int, int> enemyCellIdx = m_GridComponent->GetIndexFromPosition(enemyPos);

	// First one is child, second is parent
	std::unordered_map<Cell*, Cell*> cellTree;
	std::queue<Cell*> toVisit;
	std::unordered_set<Cell*> visited;

	Cell* startCell = m_GridComponent->GetCell(m_GridComponent->GetIndexFromPosition(enemyPos));

	toVisit.push(startCell);
	visited.insert(startCell);

	while (!toVisit.empty())
	{
		Cell* current = toVisit.front();
		toVisit.pop();

		std::pair<int, int> currentIndex = m_GridComponent->GetIndexFromPosition(current->topLeft);

		std::vector<std::pair<Cell*, bool>> neighbors = {
			{ m_GridComponent->GetCell(currentIndex.first - 1, currentIndex.second), current->canGoUp },
			{ m_GridComponent->GetCell(currentIndex.first + 1, currentIndex.second), current->canGoDown },
			{ m_GridComponent->GetCell(currentIndex.first, currentIndex.second - 1), current->canGoLeft },
			{ m_GridComponent->GetCell(currentIndex.first, currentIndex.second + 1), current->canGoRight }
		};

		for (auto& [neighbor, canMove] : neighbors)
		{
			if (neighbor && canMove && visited.find(neighbor) == visited.end())
			{
				cellTree[neighbor] = current;
				visited.insert(neighbor);
				toVisit.push(neighbor);
			}
		}
	}

	return cellTree;
}

void GroundComponent::UpdateConnectionsWithNeighbors(Cell* cell)
{
	std::pair<int, int> cellIdx = m_GridComponent->GetIndexFromPosition(cell->topLeft);
	Cell* upCell = m_GridComponent->GetCell(cellIdx.first - 1, cellIdx.second);
	Cell* downCell = m_GridComponent->GetCell(cellIdx.first + 1, cellIdx.second);
	Cell* leftCell = m_GridComponent->GetCell(cellIdx.first, cellIdx.second - 1);
	Cell* rightCell = m_GridComponent->GetCell(cellIdx.first, cellIdx.second + 1);
	upCell->canGoDown = cell->canGoUp;
	downCell->canGoUp = cell->canGoDown;
	leftCell->canGoRight = cell->canGoLeft;
	rightCell->canGoLeft = cell->canGoRight;
}