#include "Component.h"
#include "GameObject.h"

Twengine::Component::Component(GameObject* owner)
	: m_Owner{owner}
{
	m_Transform = owner->GetTransform();
}
