#include "SceneManager.h"

void Twengine::SceneManager::Start()
{
	if (m_CurrentScene != nullptr) m_CurrentScene->Start();
}

void Twengine::SceneManager::Update()
{
	if (m_CurrentScene != nullptr) m_CurrentScene->Update();
}

void Twengine::SceneManager::FixedUpdate()
{
	if (m_CurrentScene != nullptr) m_CurrentScene->FixedUpdate();
}

void Twengine::SceneManager::LateUpdate()
{
	if (m_CurrentScene != nullptr) m_CurrentScene->LateUpdate();
}


void Twengine::SceneManager::Render() const
{
	if (m_CurrentScene != nullptr) m_CurrentScene->Render();
}

void Twengine::SceneManager::RenderUI()
{
	if (m_CurrentScene != nullptr) m_CurrentScene->RenderUI();
}

void Twengine::SceneManager::SetCurrentScene(const std::string& name)
{
	for (const auto& scene : m_Scenes)
	{
		if (scene->GetName() == name)
		{
			m_CurrentScene = scene.get();
			return;
		}
	}
}

Twengine::Scene& Twengine::SceneManager::CreateScene(const std::string& name)
{
	m_Scenes.emplace_back(std::make_unique<Scene>(name));
	return *m_Scenes.back().get();
}
