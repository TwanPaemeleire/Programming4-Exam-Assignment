#pragma once
#include "GameObjectCommand.h"
class PointComponent;

class IncreaseScoreCommand : public Twengine::GameObjectCommand
{
public:
	IncreaseScoreCommand(Twengine::GameObject* gameObject);
	virtual void Execute() override;

	void SetPointIncrease(int points) { m_PointIncrease = points; }
private:
	PointComponent* m_PointComponent;
	int m_PointIncrease{};
};

