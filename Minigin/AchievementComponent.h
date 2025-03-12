#pragma once
#include "Component.h"
#include "Observer.h"

class AchievementComponent : public Component, public Observer
{
public:
	AchievementComponent(GameObject* owner);
	virtual ~AchievementComponent() override = default;
	AchievementComponent(const AchievementComponent& other) = delete;
	AchievementComponent(AchievementComponent&& other) = delete;
	AchievementComponent& operator=(const AchievementComponent& other) = delete;
	AchievementComponent& operator=(AchievementComponent&& other) = delete;


	virtual void Notify(GameEvent event, GameObject* observedObject) override;
};

