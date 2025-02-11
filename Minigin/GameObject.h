#pragma once
#include <memory>
#include <vector>
#include "TransformComponent.h"

class Component;
class Texture2D;

class GameObject final
{
public:
	virtual void Start();
	virtual void Update();
	virtual void FixedUpdate();
	virtual void LateUpdate();
	virtual void Render() const;

	void SetPosition(float x, float y);

	void AddComponent(std::unique_ptr<Component> component);
	template <typename T>
	T* GetComponent() const;

	void MarkForDestruction() { m_MarkedForDestruction = true; }
	bool IsMarkedForDestruction() const { return m_MarkedForDestruction; }

	GameObject();
	virtual ~GameObject() {};
	GameObject(const GameObject& other) = delete;
	GameObject(GameObject&& other) = delete;
	GameObject& operator=(const GameObject& other) = delete;
	GameObject& operator=(GameObject&& other) = delete;

private:
	bool m_MarkedForDestruction{ false };

	TransformComponent* m_Transform{};
	std::vector<std::unique_ptr<Component>> m_Components;
};


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