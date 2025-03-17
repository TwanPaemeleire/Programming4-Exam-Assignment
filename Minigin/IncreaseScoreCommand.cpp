#include "IncreaseScoreCommand.h"
#include "GameObject.h"
#include "PointComponent.h"

IncreaseScoreCommand::IncreaseScoreCommand(Twengine::GameObject* gameObject)
	:GameObjectCommand(gameObject)
{
	m_PointComponent = gameObject->GetComponent<PointComponent>();
}

void IncreaseScoreCommand::Execute()
{
	m_PointComponent->ChangePoints(m_PointIncrease);
}
