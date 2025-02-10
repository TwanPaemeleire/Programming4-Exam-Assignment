#pragma once
class GameObject;
class Transform;

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
	virtual void Render() const {};

	GameObject* GetParentGameObject() const { return m_ParentGameObject; };
	Transform* GetTransform() const { return m_Transform; };

private:
	friend class GameObject;
	GameObject* m_ParentGameObject = nullptr;
	Transform* m_Transform = nullptr;
};

