#pragma once
#include "Component.h"

class GroundComponent final : public Twengine::Component
{
public:
	GroundComponent(Twengine::GameObject* owner);
	virtual ~GroundComponent() override = default;
	GroundComponent(const GroundComponent& other) = delete;
	GroundComponent(GroundComponent&& other) = delete;
	GroundComponent& operator=(const GroundComponent& other) = delete;
	GroundComponent& operator=(GroundComponent&& other) = delete;

private:

};

