#pragma once
#include "Component.h"
#include <memory>
#include "Observer.h"
#include <glm.hpp>

namespace Twengine
{
	class Event;
	class GameObject;
}

class ScoreComponent final : public Twengine::Component, public Twengine::Observer
{
public:
	ScoreComponent(Twengine::GameObject* owner);
	virtual ~ScoreComponent() = default;
	ScoreComponent(const ScoreComponent& other) = delete;
	ScoreComponent(ScoreComponent&& other) = delete;
	ScoreComponent& operator=(const ScoreComponent& other) = delete;
	ScoreComponent& operator=(ScoreComponent&& other) = delete;

	void SetScore(int points);
	void IncreaseScore(int scoreIncrease);
	int GetScore() const { return m_Score; }

	virtual void Notify(const GameEvent& event, Twengine::GameObject* observedObject) override;

	Twengine::Event* GetScoreChangedEvent() const { return m_ScoreChangedEvent.get(); }

private:
	int GetEnemyLayer(const glm::vec2& pos) const;
	int m_Score{};
	std::unique_ptr<Twengine::Event> m_ScoreChangedEvent;
};

