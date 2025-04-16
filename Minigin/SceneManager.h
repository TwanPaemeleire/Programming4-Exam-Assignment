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

		void SetCurrentScene(const std::string& name);
		Scene& GetCurrentScene() const { return *m_CurrentScene; }
		Scene& CreateScene(const std::string& name);
	private:
		Scene* m_CurrentScene{ nullptr };
		friend class Singleton<SceneManager>;
		SceneManager() = default;
		std::vector<std::unique_ptr<Scene>> m_Scenes;
	};
}