#pragma once
#include <vector>
#include <string>
#include <memory>
#include "Singleton.h"
#include "Scene.h"

namespace Twengine
{
	class SceneManager final : public Singleton<SceneManager>
	{
	public:

		void Start();
		void Update();
		void FixedUpdate();
		void LateUpdate();
		void Render() const;
		void RenderUI();

		void RequestSetCurrentScene(const std::string& name);
		void RequestSetCurrentScene(unsigned int id);
		Scene& GetCurrentScene() const { return *m_CurrentScene; }
		Scene& GetPersistentScene() const { return *m_PersistentScene; }
		Scene& CreateScene(const std::string& name, std::function<void()> loadFunction, bool isPersistent = false);
		void ClearPersistentScene();
	private:
		void SetCurrentScene(const std::string& name);

		Scene* m_CurrentScene{ nullptr };
		std::unique_ptr<Scene> m_PersistentScene{};
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::vector<std::unique_ptr<Scene>> m_Scenes;

		bool m_ShouldSetScene{ false };
		std::string m_SceneToSet{};
	};
}