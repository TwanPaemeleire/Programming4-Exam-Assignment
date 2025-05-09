#include "RectColliderComponent.h"
#include "Renderer.h"
#include "GameObject.h"

std::vector<Twengine::RectColliderComponent*> Twengine::RectColliderComponent::s_Colliders = {};

Twengine::RectColliderComponent::RectColliderComponent(GameObject* owner)
	:Component(owner)
{
	m_HitBox = std::make_unique<RectHitbox>();
	m_OnCollisionEvent = std::make_unique<Event>();
	owner->GetComponent<TransformComponent>()->GetOnPositionChangedEvent()->AddObserver(this);
	s_Colliders.push_back(this);
}

Twengine::RectColliderComponent::~RectColliderComponent()
{
	s_Colliders.erase(std::remove(s_Colliders.begin(), s_Colliders.end(), this), s_Colliders.end());
}

void Twengine::RectColliderComponent::Render() const
{
	glm::vec2& topLeft = m_HitBox->topLeft;
	Renderer::GetInstance().DrawRectangle(topLeft.x, topLeft.y, m_HitBox->width, m_HitBox->height, SDL_Color(0, 255, 0, 255));
}

void Twengine::RectColliderComponent::FixedUpdate()
{
	if (!m_Enabled) return;
	for (std::size_t colliderIdx{ 0 }; colliderIdx < s_Colliders.size(); ++colliderIdx)
	{
		if (this == s_Colliders[colliderIdx] || !s_Colliders[colliderIdx]->GetEnabled()) continue;

		if (IsOverlapping(s_Colliders[colliderIdx]))
		{
			m_OnCollisionEvent->NotifyObservers(GameEvent(make_sdbm_hash("OnCollision")), s_Colliders[colliderIdx]->GetOwner());
			s_Colliders[colliderIdx]->GetOnCollisionEvent()->NotifyObservers(GameEvent(make_sdbm_hash("OnCollision")), GetOwner());
		}
	}
}

void Twengine::RectColliderComponent::Notify(const GameEvent& event, GameObject* observedObject)
{
	if (event.id == make_sdbm_hash("TransformPositionChanged"))
	{
		m_HitBox->topLeft = observedObject->GetTransform()->GetWorldPosition();
	}
}

bool Twengine::RectColliderComponent::IsOverlapping(RectColliderComponent* other) const
{
	RectHitbox* otherHitBox = other->GetHitBox();
	glm::vec2& otherTopLeft = otherHitBox->topLeft;
	glm::vec2& myTopLeft = m_HitBox->topLeft;
	return (myTopLeft.x < otherTopLeft.x + otherHitBox->width	&& // My left is left of other's right
			myTopLeft.x + m_HitBox->width > otherTopLeft.x		&& // My right is right of other's left
			myTopLeft.y < otherTopLeft.y + otherHitBox->height	&& // My top is above other's bottom
			myTopLeft.y + m_HitBox->height > otherTopLeft.y);	   // My bottom is below other's top
}

void Twengine::RectColliderComponent::SetHitBox(glm::vec2 topLeft, float width, float height)
{
	m_HitBox->topLeft = topLeft;
	m_HitBox->width = width;
	m_HitBox->height = height;
}
