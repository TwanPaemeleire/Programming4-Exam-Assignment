#include "SceneManager.h"

void Twengine::SceneManager::Start()
{
	m_PersistentScene->Start();
	for (auto& scene : m_Scenes)
	{
		scene->Start();
	}
}

void Twengine::SceneManager::Update()
{
	if (m_ShouldSetScene)
	{
		SetCurrentScene(m_SceneToSet);
		m_ShouldSetScene = false;
	}
	m_PersistentScene->Update();
	for (auto& scene : m_Scenes)
	{
		scene->Update();
	}
}

void Twengine::SceneManager::FixedUpdate()
{
	m_PersistentScene->FixedUpdate();
	for (auto& scene : m_Scenes)
	{
		scene->FixedUpdate();
	}
}

void Twengine::SceneManager::LateUpdate()
{
	m_PersistentScene->LateUpdate();
	for (auto& scene : m_Scenes)
	{
		scene->LateUpdate();
	}
}

void Twengine::SceneManager::Render() const
{
	m_PersistentScene->Render();
	for (auto& scene : m_Scenes)
	{
		scene->Render();
	}
}

void Twengine::SceneManager::RenderUI()
{
	m_PersistentScene->RenderUI();
	for (auto& scene : m_Scenes)
	{
		scene->RenderUI();
	}
}

void Twengine::SceneManager::RequestSetCurrentScene(const std::string& name)
{
	m_ShouldSetScene = true;
	m_SceneToSet = name;
}

void Twengine::SceneManager::RequestSetCurrentScene(unsigned int id)
{
	RequestSetCurrentScene(m_Scenes[id]->GetName());
}

Twengine::Scene& Twengine::SceneManager::CreateScene(const std::string& name, std::function<void()> loadFunction, bool isPersistent)
{
	if (isPersistent)
	{
		m_PersistentScene = std::make_unique<Scene>(name, loadFunction, isPersistent);
		m_PersistentScene->Load();
		return *m_PersistentScene.get();
	}
	m_Scenes.emplace_back(std::make_unique<Scene>(name, loadFunction, isPersistent));
	return *m_Scenes.back().get();
}

void Twengine::SceneManager::ClearPersistentScene()
{
	if(m_PersistentScene) m_PersistentScene->RemoveAll();
}

void Twengine::SceneManager::SetCurrentScene(const std::string& name)
{
	for (const auto& scene : m_Scenes)
	{
		if (scene->GetName() == name)
		{
			Scene* prevScene = m_CurrentScene;
			m_CurrentScene = scene.get();
			m_CurrentScene->Load();
			if (prevScene != nullptr)
			{
				prevScene->ResetHasStarted();
				prevScene->RemoveAll();
			}
			m_CurrentScene->Start();
			m_PersistentScene->Start();
			return;
		}
	}
}
