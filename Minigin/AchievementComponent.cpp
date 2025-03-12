#include "AchievementComponent.h"
#include "GameObject.h"
#include "PointComponent.h"
#include <steam_api.h>

AchievementComponent::AchievementComponent(GameObject* owner)
	:Component(owner)
{
}

void AchievementComponent::Notify(GameEvent event, GameObject* observedObject)
{
	if (event == GameEvent::PointsChanged)
	{
		if (observedObject->GetComponent<PointComponent>()->GetPoints() >= 500)
		{
			SteamUserStats()->SetAchievement("ACH_WIN_ONE_GAME");
			SteamUserStats()->StoreStats();
		}
	}
}