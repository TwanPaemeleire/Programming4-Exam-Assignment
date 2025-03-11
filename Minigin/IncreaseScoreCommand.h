#pragma once
#include "GameObjectCommand.h"
class PointComponent;

class IncreaseScoreCommand : public GameObjectCommand
{
public:
	IncreaseScoreCommand(GameObject* gameObject);
	virtual void Execute() override;

	void SetPointIncrease(int points) { m_PointIncrease = points; }
private:
	PointComponent* m_PointComponent;
	int m_PointIncrease{};
};

