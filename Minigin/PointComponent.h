#pragma once
#include "Component.h"
#include <memory>

namespace Twengine
{
	class Event;
}

class PointComponent final : public Twengine::Component
{
public:
	PointComponent(Twengine::GameObject* owner);
	virtual ~PointComponent() = default;
	PointComponent(const PointComponent& other) = delete;
	PointComponent(PointComponent&& other) = delete;
	PointComponent& operator=(const PointComponent& other) = delete;
	PointComponent& operator=(PointComponent&& other) = delete;

	void ChangePoints(int amount);
	void SetPoints(int points);
	int GetPoints() const { return m_Points; }

	Twengine::Event* GetScoreChangedEvent() const { return m_ScoreChangedEvent.get(); }
private:
	int m_Points{};
	std::unique_ptr<Twengine::Event> m_ScoreChangedEvent;
};

