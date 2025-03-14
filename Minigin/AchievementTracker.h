#pragma once
#include "Singleton.h"
#include "Observer.h"

class AchievementTracker final : public Singleton<AchievementTracker>, public Observer
{
public:
	virtual void Notify(GameEvent event, GameObject* observedObject) override;
private:
	void Unlock(const char* achievement);
};

