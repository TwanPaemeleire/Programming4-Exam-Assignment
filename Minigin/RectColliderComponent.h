#pragma once
#include "Component.h"
#include "glm.hpp"
#include <memory>
#include "Event.h"
#include <utility>

namespace Twengine
{
	struct RectHitbox
	{
		glm::vec2 topLeft{};
		float width{};
		float height{};
	};

	class RectColliderComponent final : public Component
	{
	public:
		RectColliderComponent(GameObject* owner);
		virtual ~RectColliderComponent() override;
		RectColliderComponent(const RectColliderComponent& other) = delete;
		RectColliderComponent(RectColliderComponent&& other) = delete;
		RectColliderComponent& operator=(const RectColliderComponent& other) = delete;
		RectColliderComponent& operator=(RectColliderComponent&& other) = delete;

		virtual void FixedUpdate() override;
		virtual void Render() const override;

		bool IsOverlapping(RectColliderComponent* other) const;
		void Enable() { m_Enabled = true; }
		void Disable() { m_Enabled = false; }

		bool IsEnabled() const { return m_Enabled; }
		void ChangeHitBox(glm::vec2 topLeft, float width, float height);
		RectHitbox* GetHitBox() const { return m_HitBox.get(); }

		Event* GetOnCollisionEvent() const { return m_OnCollisionEvent.get(); }

		static void ProcessCollisions();
	private:
		static std::vector<RectColliderComponent*> s_Colliders;
		static std::vector<std::pair<RectColliderComponent*, RectColliderComponent*>> s_LastFrameCollisions;

		std::unique_ptr<RectHitbox> m_HitBox;
		std::unique_ptr<Twengine::Event> m_OnCollisionEvent;
		bool m_Enabled{ true };
	};
}