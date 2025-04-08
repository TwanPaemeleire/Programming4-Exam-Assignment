#pragma once
#include "Component.h"
#include <SDL_image.h>
#include <glm.hpp>

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

	void ErasePlayerTrail(SDL_Rect playerRect);
	bool PositionIsDugOut(const glm::vec2& pos);
private:
	SDL_Surface* m_Surface{};
	SDL_Texture* m_Texture{};
	Uint32 m_TransparentValue{};

};

