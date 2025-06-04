#pragma once
#include "Component.h"
#include "RockStates.h"
#include "Observer.h"

namespace Twengine
{
	class TextureRenderComponent;
}

class RockComponent final : public Twengine::Component, public Twengine::Observer
{
public:
	RockComponent(Twengine::GameObject* owner);
	virtual ~RockComponent() override = default;
	RockComponent(const RockComponent& other) = delete;
	RockComponent(RockComponent&& other) = delete;
	RockComponent& operator=(const RockComponent& other) = delete;
	RockComponent& operator=(RockComponent&& other) = delete;

	virtual void Start() override;
	virtual void Update() override;

	glm::vec2 GetBottomMiddle() const;
	void Notify(const GameEvent& event, Twengine::GameObject* observedObject) override;

private:

	void CheckAndTransitionStates(std::unique_ptr<RockState> newState);
	std::unique_ptr<RockState> m_CurrentState{};
	glm::vec2 m_Size{};

};

