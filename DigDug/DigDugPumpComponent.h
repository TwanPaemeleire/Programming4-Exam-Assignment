#pragma once
#include "Component.h"
#include "Observer.h"
#include <glm.hpp>
#include <memory>

namespace Twengine
{
	class Texture2D;
	class RectColliderComponent;
	class Event;
}

class GroundComponent;

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
	virtual void RenderUI() override;

	void OnPumpButtonInteraction(bool isPressBound);
	void Retract();
	bool IsStuckInEnemy() const { return m_IsStuckInEnemy; }
	void FlipHorizontally() { m_HorizontallyFlipped = true; }
	void ShotVertically() { m_ShotVertically = true; }

	Twengine::Event* GetOnPumpRetractedEvent() const { return m_OnPumpRetractedEvent.get(); }
	Twengine::Event* GetOnPumpEvent() const { return m_OnPumpEvent.get(); }
	virtual void Notify(const GameEvent& event, Twengine::GameObject* observedObject) override;

private:
	void SetHitBoxBasedOnDirection();
	bool NextPosIsDugOut();
	void SetDrawPosition();

	std::unique_ptr<Twengine::Event> m_OnPumpRetractedEvent{};
	std::unique_ptr<Twengine::Event> m_OnPumpEvent{};
	Twengine::RectColliderComponent* m_RectColliderComponent{};
	Twengine::Texture2D* m_PumpTexture{};

	glm::vec2 m_TextureSize{};
	float m_ExposedAmount{};
	const float m_PumpShootSpeed{80.f};
	bool m_IsReturning{ false };
	bool m_PumpButtonIsPressed{ false };
	bool m_IsStuckInEnemy{false};

	const float m_PumpDelay{ 1.0f };
	const int m_DirtLeeway{ 5 };
	float m_PumpDelayCounter{};
	glm::vec2 m_DrawPosition{};

	GroundComponent* m_GroundComponent{};
	bool m_HorizontallyFlipped{ false };
	bool m_ShotVertically{false};
	float m_ExposedAmountToCheck{};
};

