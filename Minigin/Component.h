#pragma once
class GameObject;
class TransformComponent;

class Component
{
public:
	Component(GameObject* owner);
	virtual ~Component() = default;
	//Will Deleting The Beneath Here Also Delete Them In Deriving Classes?
	Component(const Component& other) = delete;
	Component(Component&& other) = delete;
	Component& operator=(const Component& other) = delete;
	Component& operator=(Component&& other) = delete;

	virtual void Start() {};
	virtual void Update() = 0;
	virtual void FixedUpdate() {};
	virtual void LateUpdate() {};
	virtual void Render() const {};

	void MarkForDestruction() { m_MarkedForDestruction = true; }
	bool IsMarkedForDestruction() const { return m_MarkedForDestruction; }

protected:
	GameObject* GetOwner() const { return m_Owner; }
	// For Easy Access To The Owner's Transform
	TransformComponent* m_Transform = nullptr;

private:
	bool m_MarkedForDestruction{ false };
	GameObject* m_Owner = nullptr;
};

