#pragma once
#include "SceneManager.h"
#include "GameObject.h"

class Scene final
{
	friend Scene& SceneManager::CreateScene(const std::string& name);
public:
	~Scene();
	Scene(const Scene& other) = delete;
	Scene(Scene&& other) = delete;
	Scene& operator=(const Scene& other) = delete;
	Scene& operator=(Scene&& other) = delete;

	void Add(std::unique_ptr<GameObject> object);
	void Remove(std::unique_ptr<GameObject> object);
	void RemoveAll();

	void Start();
	void Update();
	void FixedUpdate();
	void LateUpdate();
	void Render() const;
	void RenderUI();


private:
	explicit Scene(const std::string& name);

	std::string m_Name;
	std::vector <std::unique_ptr<GameObject>> m_Objects{};

	static unsigned int m_idCounter;
};
