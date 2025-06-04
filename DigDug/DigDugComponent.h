#pragma once
#include "Component.h"
#include <memory>
#include "PlayerStates.h"

namespace Twengine
{
	class Texture2D;
	class RectColliderComponent;
};

struct GameEvent;

struct PlayerMovingData
{
	glm::vec2 direction = { 0.f, 0.f };
	glm::vec2 lastNonNullDirection;
	glm::vec2 targetPosition = { -1.f, -1.f };
	glm::vec2 currentInputDirection = { 0.f, 0.f };
	float distanceToTarget{};
	float distanceTracker{};
	std::pair<int, int> currentIndex{};
	bool isMoving{ false };
};

class DigDugComponent : public Twengine::Component, public Twengine::Observer
{
public:
	DigDugComponent(Twengine::GameObject* owner);
	virtual ~DigDugComponent() override = default;
	DigDugComponent(const DigDugComponent& other) = delete;
	DigDugComponent(DigDugComponent&& other) = delete;
	DigDugComponent& operator=(const DigDugComponent& other) = delete;
	DigDugComponent& operator=(DigDugComponent&& other) = delete;

	virtual void Start() override;
	virtual void Update() override;
	virtual void RenderUI() override;

	void SetDirection(glm::vec2 dir);
	void OnPumpButtonInteraction(bool pressBound);
	virtual void Notify(const GameEvent& event, Twengine::GameObject* observedObject) override;

	PlayerMovingData* GetPlayerMovingData() const { return m_PlayerMovingData.get(); }

private:
	void CheckAndTransitionStates(std::unique_ptr<PlayerState> newState);
	std::unique_ptr<PlayerState> m_CurrentState{};
	std::unique_ptr<PlayerMovingData> m_PlayerMovingData{};
	Twengine::AnimationComponent* m_AnimationComponent{};
	Twengine::RectColliderComponent* m_RectColliderComponent{};
};

