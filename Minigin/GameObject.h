#pragma once
#include <memory>
#include <vector>
#include "TransformComponent.h"

class Component;
class Texture2D;

class GameObject final
{
public:
	GameObject();
	virtual ~GameObject() {};
	GameObject(const GameObject& other) = delete;
	GameObject(GameObject&& other) = delete;
	GameObject& operator=(const GameObject& other) = delete;
	GameObject& operator=(GameObject&& other) = delete;

	void Start();
	void Update();
	void FixedUpdate();
	void LateUpdate();
	void Render() const;

	void SetPosition(float x, float y);

	template <typename T>
	T* AddComponent();
	template <typename T>
	T* GetComponent() const;
	template <typename T>
	bool HasComponent() const;

	void MarkForDestruction() { m_MarkedForDestruction = true; }
	bool IsMarkedForDestruction() const { return m_MarkedForDestruction; }

private:
	bool m_MarkedForDestruction{ false };
	TransformComponent* m_Transform{};
	std::vector<std::unique_ptr<Component>> m_Components;
};


template<typename T>
inline T* GameObject::AddComponent()
{
	static_assert(std::is_base_of<Component, T>::value, "T passed to AddComponent<>() does NOT inherit from Component");
	std::unique_ptr<T> component = std::make_unique<T>(this, m_Transform);
	T* rawPtr = component.get();
	m_Components.emplace_back(std::move(component));
	return rawPtr;
}

template <typename T>
inline T* GameObject::GetComponent() const
{
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
inline bool GameObject::HasComponent() const
{
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
