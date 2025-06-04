#pragma once
#include <memory>
#include <vector>
#include "TransformComponent.h"
#include "SdbmHash.h"

using TagId = unsigned int;

namespace Twengine
{
	class GameObject final
	{
	public:
		GameObject();
		~GameObject();
		GameObject(const GameObject& other) = delete;
		GameObject(GameObject&& other) = delete;
		GameObject& operator=(const GameObject& other) = delete;
		GameObject& operator=(GameObject&& other) = delete;

		void Start();
		void Update();
		void FixedUpdate();
		void LateUpdate();
		void Render() const;
		void RenderUI();

		TransformComponent* GetTransform() { return m_Transform; }

		template <typename T>
		T* AddComponent();
		template <typename T>
		T* GetComponent() const;
		template <typename T>
		bool HasComponent() const;

		void MarkForDestruction();
		bool IsMarkedForDestruction() const { return m_MarkedForDestruction; }

		void SetParent(GameObject* parent, bool keepWorldPosition);
		GameObject* GetParent() const { return m_Parent; }
		size_t GetChildCount() const { return m_Children.size(); }
		GameObject* GetChildAt(int index) const { return m_Children[index]; };

		void SetTag(TagId newTag) {m_TagId = newTag; }
		TagId GetTag() const { return m_TagId; }

		void Deactivate() { m_IsActive = false; }
		void Activate() { m_IsActive = true; }
		bool IsActive() const { return m_IsActive; }

	private:
		void AddChild(GameObject* child);
		void RemoveChild(GameObject* child);
		bool IsChild(GameObject* objectToCheck);

		bool m_MarkedForDestruction{ false };
		bool m_IsActive{true};

		TransformComponent* m_Transform{};

		std::vector<std::unique_ptr<Component>> m_Components;
		GameObject* m_Parent;
		std::vector<GameObject*> m_Children;

		TagId m_TagId{};
	};
}

template<typename T>
inline T* Twengine::GameObject::AddComponent()
{
	static_assert(std::is_base_of<Component, T>::value, "T passed to AddComponent<>() does NOT inherit from Component");
	std::unique_ptr<T> component = std::make_unique<T>(this);
	T* rawPtr = component.get();
	m_Components.emplace_back(std::move(component));
	return rawPtr;
}

template <typename T>
inline T* Twengine::GameObject::GetComponent() const
{
	static_assert(std::is_base_of<Component, T>::value, "T passed to GetComponent<>() does NOT inherit from Component");
	for (auto& component : m_Components)
	{
		T* tempComponent = dynamic_cast<T*>(component.get());
		if (tempComponent != nullptr)
		{
			return tempComponent;
		}
	}
	return nullptr;
}

template<typename T>
inline bool Twengine::GameObject::HasComponent() const
{
	static_assert(std::is_base_of<Component, T>::value, "T passed to HasComponent<>() does NOT inherit from Component");
	for (auto& component : m_Components)
	{
		T* tempComponent = dynamic_cast<T*>(component.get());
		if (tempComponent != nullptr)
		{
			return true;
		}
	}
	return false;
}
