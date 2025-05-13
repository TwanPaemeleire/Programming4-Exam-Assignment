#include "RectColliderComponent.h"
#include "Renderer.h"
#include "GameObject.h"

std::vector<Twengine::RectColliderComponent*> Twengine::RectColliderComponent::s_Colliders = {};
std::vector<std::pair<Twengine::RectColliderComponent*, Twengine::RectColliderComponent*>> Twengine::RectColliderComponent::s_LastFrameCollisions = {};

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

	// Make sure no dangling pointers remain in the last frame collisions vector
	s_LastFrameCollisions.erase(
		std::remove_if(s_LastFrameCollisions.begin(), s_LastFrameCollisions.end(),
			[this](const std::pair<RectColliderComponent*, RectColliderComponent*>& pair)
			{
				return pair.first == this || pair.second == this;
			}),
		s_LastFrameCollisions.end());
}

void Twengine::RectColliderComponent::Render() const
{
	glm::vec2& topLeft = m_HitBox->topLeft;
	Renderer::GetInstance().DrawRectangle(topLeft.x, topLeft.y, m_HitBox->width, m_HitBox->height, SDL_Color(0, 255, 0, 255));
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

void Twengine::RectColliderComponent::ProcessCollisions()
{
	std::vector<std::pair<RectColliderComponent*, RectColliderComponent*>> currentCollisions;

	for (size_t firstIndex = 0; firstIndex < s_Colliders.size(); ++firstIndex)
	{
		RectColliderComponent* first = s_Colliders[firstIndex];
		if (!first->GetEnabled()) continue;

		for (size_t secondIndex = firstIndex + 1; secondIndex < s_Colliders.size(); ++secondIndex)
		{
			RectColliderComponent* second = s_Colliders[secondIndex];
			if (!second->GetEnabled()) continue;

			if (first->IsOverlapping(second))
			{
				auto pair = std::make_pair(first, second);
				currentCollisions.push_back(pair);

				// Called every frame when colliding
				first->m_OnCollisionEvent->NotifyObservers(GameEvent(make_sdbm_hash("OnCollision")), second->GetOwner());
				second->m_OnCollisionEvent->NotifyObservers(GameEvent(make_sdbm_hash("OnCollision")), first->GetOwner());

				// Called only when starting collision
				if (std::find(s_LastFrameCollisions.begin(), s_LastFrameCollisions.end(), pair) == s_LastFrameCollisions.end())
				{
					first->m_OnCollisionEvent->NotifyObservers(GameEvent(make_sdbm_hash("OnCollisionEnter")), second->GetOwner());
					second->m_OnCollisionEvent->NotifyObservers(GameEvent(make_sdbm_hash("OnCollisionEnter")), first->GetOwner());
				}
			}
		}
	}
	// Called only when ending collision
	for (const auto& lastFramePair : s_LastFrameCollisions)
	{
		if (std::find(currentCollisions.begin(), currentCollisions.end(), lastFramePair) == currentCollisions.end())
		{
			RectColliderComponent* first = lastFramePair.first;
			RectColliderComponent* second = lastFramePair.second;

			if (first->GetEnabled())
			{
				first->m_OnCollisionEvent->NotifyObservers(GameEvent(make_sdbm_hash("OnCollisionExit")), second->GetOwner());
			}
			if (second->GetEnabled())
			{
				second->m_OnCollisionEvent->NotifyObservers(GameEvent(make_sdbm_hash("OnCollisionExit")), first->GetOwner());
			}
		}
	}

	// Update vector with last frame collisions
	s_LastFrameCollisions = std::move(currentCollisions);
}
