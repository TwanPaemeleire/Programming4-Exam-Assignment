#include "SceneManager.h"

void Twengine::SceneManager::Start()
{
	//if (m_CurrentScene != nullptr) m_CurrentScene->Start();
	m_PersistentScene->Start();
	for (auto& scene : m_Scenes)
	{
		scene->Start();
	}
}

void Twengine::SceneManager::Update()
{
	m_PersistentScene->Update();
	//if (m_CurrentScene != nullptr) m_CurrentScene->Update();
	for (auto& scene : m_Scenes)
	{
		scene->Update();
	}
}

void Twengine::SceneManager::FixedUpdate()
{
	m_PersistentScene->FixedUpdate();
	//if (m_CurrentScene != nullptr) m_CurrentScene->FixedUpdate();
	for (auto& scene : m_Scenes)
	{
		scene->FixedUpdate();
	}
}

void Twengine::SceneManager::LateUpdate()
{
	m_PersistentScene->LateUpdate();
	//if (m_CurrentScene != nullptr) m_CurrentScene->LateUpdate();
	for (auto& scene : m_Scenes)
	{
		scene->LateUpdate();
	}
}

void Twengine::SceneManager::Render() const
{
	m_PersistentScene->Render();
	//if (m_CurrentScene != nullptr) m_CurrentScene->Render();
	for (auto& scene : m_Scenes)
	{
		scene->Render();
	}
}

void Twengine::SceneManager::RenderUI()
{
	m_PersistentScene->RenderUI();
	//if (m_CurrentScene != nullptr) m_CurrentScene->RenderUI();
	for (auto& scene : m_Scenes)
	{
		scene->RenderUI();
	}
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
