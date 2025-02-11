#pragma once
class GameObject;
class TransformComponent;

class Component
{
public:
	Component() = default;
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

	void MarkForDestruction() { m_MarkedForDestruction = true; }
	bool IsMarkedForDestruction() const { return m_MarkedForDestruction; }

	GameObject* GetParentGameObject() const { return m_ParentGameObject; }
	TransformComponent* GetTransform() const { return m_Transform; }

private:
	bool m_MarkedForDestruction{ false };

	friend class GameObject;
	GameObject* m_ParentGameObject = nullptr;
	TransformComponent* m_Transform = nullptr;
};

