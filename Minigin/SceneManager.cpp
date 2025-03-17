#include "SceneManager.h"
#include "Scene.h"

void Twengine::SceneManager::Start()
{
	for (auto& scene : m_Scenes)
	{
		scene->Start();
	}
}

void Twengine::SceneManager::Update()
{
	for(auto& scene : m_Scenes)
	{
		scene->Update();
	}
}

void Twengine::SceneManager::FixedUpdate()
{
	for (auto& scene : m_Scenes)
	{
		scene->FixedUpdate();
	}
}

void Twengine::SceneManager::LateUpdate()
{
	for (auto& scene : m_Scenes)
	{
		scene->LateUpdate();
	}
}


void Twengine::SceneManager::Render() const
{
	for (const auto& scene : m_Scenes)
	{
		scene->Render();
	}
}

void Twengine::SceneManager::RenderUI()
{
	for (const auto& scene : m_Scenes)
	{
		scene->RenderUI();
	}
}

Twengine::Scene& Twengine::SceneManager::CreateScene(const std::string& name)
{
	const auto& scene = std::shared_ptr<Scene>(new Scene(name));
	m_Scenes.push_back(scene);
	return *scene;
}
