#include "AchievementTracker.h"
#include "GameObject.h"
#include "PointComponent.h"
#include <steam_api.h>

void AchievementTracker::Notify(GameEvent event, GameObject* observedObject)
{
	if (event == GameEvent::PointsChanged)
	{
		if (observedObject->GetComponent<PointComponent>()->GetPoints() >= 500)
		{
			Unlock("ACH_WIN_ONE_GAME");
		}
	}
}

void AchievementTracker::Unlock(const char* achievement)
{
	SteamUserStats()->SetAchievement(achievement);
	SteamUserStats()->StoreStats();
}
