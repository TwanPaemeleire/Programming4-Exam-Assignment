#pragma once
#include "GameObject.h"
#include <string>
#include <functional>

namespace Twengine
{
	class Scene final
	{
	public:
		explicit Scene(const std::string& name);
		~Scene();
		Scene(const Scene& other) = delete;
		Scene(Scene&& other) = delete;
		Scene& operator=(const Scene& other) = delete;
		Scene& operator=(Scene&& other) = delete;

		void Load();
		void Add(std::unique_ptr<GameObject> object);
		void Remove(std::unique_ptr<GameObject> object);
		void RemoveAll();

		void Start();
		void Update();
		void FixedUpdate();
		void LateUpdate();
		void Render() const;
		void RenderUI();

		const std::string& GetName() const { return m_Name; }
		void SetLoadFunction(std::function<void()> loadFunction) { m_LoadFunction = loadFunction; }
		void ResetHasStarted() { m_HasStarted = false; }

	private:
		bool m_HasStarted = false;
		std::string m_Name;
		std::vector <std::unique_ptr<GameObject>> m_Objects{};
		std::function<void()> m_LoadFunction{};

		static unsigned int m_idCounter;
	};
}