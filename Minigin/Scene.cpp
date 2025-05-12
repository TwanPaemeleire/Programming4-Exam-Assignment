#include "Scene.h"
#include "GameObject.h"

#include <algorithm>

unsigned int Twengine::Scene::m_idCounter = 0;

Twengine::Scene::Scene(const std::string& name, std::function<void()> loadFunction, bool isPersistent)
	: m_Name(name), m_IsPersistent{isPersistent}, m_LoadFunction{loadFunction}
{}

Twengine::Scene::~Scene() = default;

void Twengine::Scene::Load()
{
	m_LoadFunction();
}

void Twengine::Scene::Add(std::unique_ptr<GameObject> object)
{
	if (m_HasStarted) object->Start(); // If added during the game, call start on it again
	m_Objects.emplace_back(std::move(object));
}

void Twengine::Scene::Remove(std::unique_ptr<GameObject> object)
{
	m_Objects.erase(std::remove(m_Objects.begin(), m_Objects.end(), object), m_Objects.end());
}

void Twengine::Scene::RemoveAll()
{
	m_Objects.clear();
}

void Twengine::Scene::Start()
{
	if (m_HasStarted) return;
	for (auto& object : m_Objects)
	{
		object->Start();
	}
	m_HasStarted = true;
}

void Twengine::Scene::Update()
{
	for(auto& object : m_Objects)
	{
		object->Update();
	}
}

void Twengine::Scene::FixedUpdate()
{
	for (auto& object : m_Objects)
	{
		object->FixedUpdate();
	}
}

void Twengine::Scene::LateUpdate()
{
	for (auto& object : m_Objects)
	{
		object->LateUpdate();
	}

	// Delete the objects marked for destruction
	std::erase_if(m_Objects, [](const auto& object)
		{
			return object->IsMarkedForDestruction();
		});

}

void Twengine::Scene::Render() const
{
	for (const auto& object : m_Objects)
	{
		object->Render();
	}
}

void Twengine::Scene::RenderUI()
{
	for (const auto& object : m_Objects)
	{
		object->RenderUI();
	}
}

