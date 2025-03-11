#pragma once
#include "Component.h"
#include <memory>
class Subject;

class PointComponent : public Component
{
public:
	PointComponent(GameObject* owner);
	virtual ~PointComponent() = default;
	PointComponent(const PointComponent& other) = delete;
	PointComponent(PointComponent&& other) = delete;
	PointComponent& operator=(const PointComponent& other) = delete;
	PointComponent& operator=(PointComponent&& other) = delete;

	void ChangePoints(int amount);
	void SetPoints(int points);
	int GetPoints() const { return m_Points; }
private:
	int m_Points{};
	std::unique_ptr<Subject> m_ScoreChangedEvent;
};

