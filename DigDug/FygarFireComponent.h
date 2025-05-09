#pragma once
#include "Component.h"

namespace Twengine
{
	class AnimationComponent;
	class RectColliderComponent;
}

class FygarFireComponent : public Twengine::Component
{
public:
	FygarFireComponent(Twengine::GameObject* owner);
	virtual ~FygarFireComponent() override = default;
	FygarFireComponent(const FygarFireComponent& other) = delete;
	FygarFireComponent(FygarFireComponent&& other) = delete;
	FygarFireComponent& operator=(const FygarFireComponent& other) = delete;
	FygarFireComponent& operator=(FygarFireComponent&& other) = delete;

	virtual void Start() override;
	virtual void Update() override;

private:
	Twengine::AnimationComponent* m_AnimationComponent{};
	Twengine::RectColliderComponent* m_RectColliderComponent{};
	int m_CurrentFrameIndex{};
	float m_FrameWidth{};
	float m_FrameHeight{};
};

