#include "GroundComponent.h"
#include "GameManager.h"
#include "GameObject.h"
#include "GridComponent.h"

GroundComponent::GroundComponent(Twengine::GameObject* owner)
	: Component(owner)
{
	m_Transform->SetLocalPosition(GameManager::GetInstance().GetGrid()->GetPositionFromIndex(2, 0));
}
