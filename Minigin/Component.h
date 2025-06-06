#pragma once

namespace Twengine
{
	class GameObject;
	class TransformComponent;

	class Component
	{
	public:
		Component(GameObject* owner);
		virtual ~Component() = default;
		Component(const Component& other) = delete;
		Component(Component&& other) = delete;
		Component& operator=(const Component& other) = delete;
		Component& operator=(Component&& other) = delete;

		virtual void Start() {};
		virtual void Update() {};
		virtual void FixedUpdate() {};
		virtual void LateUpdate() {};
		virtual void Render() const {};
		virtual void RenderUI() {};

		void MarkForDestruction() { m_MarkedForDestruction = true; }
		bool IsMarkedForDestruction() const { return m_MarkedForDestruction; }
		GameObject* GetOwner() const { return m_Owner; }

	protected:
		// For easy access to the owner's transform
		TransformComponent* m_Transform = nullptr;

	private:
		bool m_MarkedForDestruction{ false };
		GameObject* m_Owner = nullptr;
	};
}