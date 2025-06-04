#pragma once
#include "Component.h"
#include <SDL_image.h>
#include <glm.hpp>
#include <vector>
#include <unordered_map>

class GridComponent;
struct Cell;

class GroundComponent final : public Twengine::Component
{
public:
	GroundComponent(Twengine::GameObject* owner);
	virtual ~GroundComponent() override = default;
	GroundComponent(const GroundComponent& other) = delete;
	GroundComponent(GroundComponent&& other) = delete;
	GroundComponent& operator=(const GroundComponent& other) = delete;
	GroundComponent& operator=(GroundComponent&& other) = delete;

	virtual void Render() const override;
	virtual void RenderUI() override;

	void ErasePlayerTrail(SDL_Rect playerRect, bool isInWorldSpace = true);
	// Will be used if I have enough time left
	void ErasePlayerTrail(int centerX, int centerY, int width, int height, bool isInWorldSpace = true);
	bool PositionIsDugOut(const glm::vec2& pos);

	glm::vec2 GetCellTargetToGetCloserToPlayer(const glm::vec2& enemyPos) const;
	bool CanMoveBetween(const glm::vec2& startPos, const glm::vec2& targetPos, int dirtLeeway = 0) const; // Dirt leeway will be used for pump & fygar fire
	bool EnemyCanReachPlayer(const glm::vec2& enemyPos) const;

	void Reset();
private:
	void CreateSurface();

	SDL_Surface* m_Surface{};
	SDL_Texture* m_Texture{};
	Uint32 m_TransparentValue{};

	GridComponent* m_GridComponent{};
	float m_GridCellSize{};
	float m_HalfGridCellSize{};

	std::unordered_map<Cell*, Cell*> BuildReachableCellTree(const glm::vec2& enemyPos) const;
	void UpdateConnectionsWithNeighbors(Cell* cell);
};

