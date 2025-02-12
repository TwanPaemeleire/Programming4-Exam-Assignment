#pragma once
class GameObject;
class TransformComponent;

class Component
{
public:
	Component() = default;
	virtual ~Component() = default;
	//Will Deleting The Beneath Here Also Delete Them In Deriving Classes?
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

	void SetParentGameObject(GameObject* parentObject) { m_ParentGameObject = parentObject; }
	GameObject* GetParentGameObject() const { return m_ParentGameObject; }
	void SetTransform(TransformComponent* transform) { m_Transform = transform; }
	TransformComponent* GetTransform() const { return m_Transform; }

private:
	bool m_MarkedForDestruction{ false };
	GameObject* m_ParentGameObject = nullptr;
	TransformComponent* m_Transform = nullptr;
};

