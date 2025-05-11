#pragma once
#include "Component.h"
#include "Observer.h"
#include <glm.hpp>

namespace Twengine
{
	class Texture2D;
	class RectColliderComponent;
}

class DigDugPumpComponent final : public Twengine::Component, public Twengine::Observer
{
public:
	DigDugPumpComponent(Twengine::GameObject* owner);
	virtual ~DigDugPumpComponent() override = default;
	DigDugPumpComponent(const DigDugPumpComponent& other) = delete;
	DigDugPumpComponent(DigDugPumpComponent&& other) = delete;
	DigDugPumpComponent& operator=(const DigDugPumpComponent& other) = delete;
	DigDugPumpComponent& operator=(DigDugPumpComponent&& other) = delete;

	virtual void Start() override;
	virtual void Update() override;
	virtual void Render() const override;

	virtual void Notify(const GameEvent& event, Twengine::GameObject* observedObject) override;

private:
	Twengine::RectColliderComponent* m_RectColliderComponent{};
	Twengine::Texture2D* m_PumpTexture{};

	glm::vec2 m_TextureSize{};
	float m_ExposedAmount{};
	float m_PumpShootSpeed{5.f};
	bool m_IsReturning{ false };
};

