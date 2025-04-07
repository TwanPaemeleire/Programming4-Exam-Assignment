#pragma once
#include <glm.hpp>
#include "Component.h"
#include <memory>
#include "Event.h"

namespace Twengine
{
	class TransformComponent final : public Component
	{
	public:
		TransformComponent(GameObject* owner);
		virtual ~TransformComponent() override = default;
		TransformComponent(const TransformComponent& other) = delete;
		TransformComponent(TransformComponent&& other) = delete;
		TransformComponent& operator=(const TransformComponent& other) = delete;
		TransformComponent& operator=(TransformComponent&& other) = delete;

		virtual void Update() override {};

		void SetLocalPosition(const glm::vec3& localPos);
		void SetLocalPosition(const glm::vec2& localPos);
		void SetLocalPosition(float x, float y);
		const glm::vec3& GetWorldPosition();
		const glm::vec3& GetLocalPosition() { return m_LocalPosition; }
		void SetPositionDirty();

		Event* GetOnPositionChangedEvent() const { return m_OnPositionChangedEvent.get(); }

	private:
		void UpdateWorldPosition();

		bool m_PositionIsDirty{ false };
		glm::vec3 m_WorldPosition{};
		glm::vec3 m_LocalPosition{};

		std::unique_ptr<Event> m_OnPositionChangedEvent;
	};
}