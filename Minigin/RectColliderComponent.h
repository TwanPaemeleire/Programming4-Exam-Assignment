#pragma once
#include "Component.h"
#include "Observer.h"
#include "glm.hpp"
#include <memory>
#include "Event.h"

namespace Twengine
{
	//class Event;

	struct RectHitbox
	{
		glm::vec2 topLeft{};
		float width{};
		float height{};
	};

	class RectColliderComponent final : public Component, public Observer
	{
	public:
		RectColliderComponent(GameObject* owner);
		virtual ~RectColliderComponent() override;
		RectColliderComponent(const RectColliderComponent& other) = delete;
		RectColliderComponent(RectColliderComponent&& other) = delete;
		RectColliderComponent& operator=(const RectColliderComponent& other) = delete;
		RectColliderComponent& operator=(RectColliderComponent&& other) = delete;

		virtual void Render() const override;
		virtual void FixedUpdate() override;

		virtual void Notify(const GameEvent& event, GameObject* observedObject) override;

		bool IsOverlapping(RectColliderComponent* other) const;
		void SetEnabled(bool enabled) { m_Enabled = enabled; }
		bool GetEnabled() const { return m_Enabled; }
		void SetHitBox(glm::vec2 topLeft, float width, float height);
		RectHitbox* GetHitBox() const { return m_HitBox.get(); }

		Event* GetOnCollisionEvent() const { return m_OnCollisionEvent.get(); }

	private:
		static std::vector<RectColliderComponent*> s_Colliders;

		std::unique_ptr<RectHitbox> m_HitBox;
		std::unique_ptr<Twengine::Event> m_OnCollisionEvent;
		bool m_Enabled{ true };
	};
}