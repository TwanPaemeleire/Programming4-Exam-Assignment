#pragma once
#include "GameObject.h"
#include <string>

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

	private:
		std::string m_Name;
		std::vector <std::unique_ptr<GameObject>> m_Objects{};

		static unsigned int m_idCounter;
	};
}