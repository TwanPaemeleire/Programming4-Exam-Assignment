#include "Component.h"
#include "GameObject.h"

Component::Component(GameObject* owner)
	: m_Owner{owner}
{
	m_Transform = owner->GetTransform();
}
