#pragma once
#include "Component.h"
#include <SDL_image.h>
#include <glm.hpp>
#include <vector>

struct Node
{
	int x, y;
	int gCost;
	int hCost;
	Node* parent;

	int FCost() const { return gCost + hCost; }

	bool operator>(const Node& other) const
	{
		return FCost() > other.FCost();
	}
};

class GridComponent;

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

	void ErasePlayerTrail(SDL_Rect playerRect, bool isInWorldSpace = true);
	bool PositionIsDugOut(const glm::vec2& pos);
	std::vector<glm::vec2> FindPath(const glm::vec2& startPos, const glm::vec2& endPos, float width, float height);
private:
	SDL_Surface* m_Surface{};
	SDL_Texture* m_Texture{};
	Uint32 m_TransparentValue{};

	GridComponent* m_GridComponent{};
	float m_GridCellSize{};
	float m_HalfGridCellSize{};
};

