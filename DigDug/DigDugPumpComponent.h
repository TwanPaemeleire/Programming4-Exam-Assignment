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

	void OnPumpButtonInteraction(bool isPressBound);

	Twengine::Event* GetOnPumpRetractedEvent() const { return m_OnPumpRetractedEvent.get(); }
	Twengine::Event* GetOnPumpEvent() const { return m_OnPumpEvent.get(); }
	virtual void Notify(const GameEvent& event, Twengine::GameObject* observedObject) override;

private:
	std::unique_ptr<Twengine::Event> m_OnPumpRetractedEvent{};
	std::unique_ptr<Twengine::Event> m_OnPumpEvent{};
	Twengine::RectColliderComponent* m_RectColliderComponent{};
	Twengine::Texture2D* m_PumpTexture{};

	glm::vec2 m_TextureSize{};
	float m_ExposedAmount{};
	float m_PumpShootSpeed{20.f};
	bool m_IsReturning{ false };
	bool m_PumpButtonIsPressed{ false };
	bool m_IsStuckInEnemy{false};

	float m_PumpDelay{ 0.5f };
	float m_PumpDelayCounter{};
};

