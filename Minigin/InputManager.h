#pragma once
#include "Singleton.h"

#include <vector>
#include <memory>

class GameObject;
#include "Controller.h"
#include "GameObjectCommand.h"

namespace Twengine
{
	enum class InteractionStates
	{
		down, up, pressed
	};

	struct BindedCommand
	{
		std::unique_ptr<GameObjectCommand> command{};
		InteractionStates interactionState{};
		int controllerIndex{}; // An Index Of -1 Will Mean That It's Binded To Keyboard
		unsigned int button{};
	};

	class InputManager final : public Singleton<InputManager>
	{
	public:
		void Init();
		bool ProcessInput();

		template <typename command>
		command* BindCommandToInput(unsigned int button, InteractionStates interactionState, GameObject* gameObject, int controllerIndex);
		template <typename command>
		void RemoveBindedCommand(int controllerIndex);

	private:
		void HandleControllerInput();
		bool HandleKeyBoardInput();

		std::vector<std::unique_ptr<Controller>> m_Controllers;
		std::vector<std::unique_ptr<BindedCommand>> m_BindedCommands;
	};
}



template<typename T>
inline T* Twengine::InputManager::BindCommandToInput(unsigned int button, Twengine::InteractionStates interactionState, GameObject* gameObject, int controllerIndex)
{
	static_assert(std::is_base_of<GameObjectCommand, T>::value, "Type passed to BindCommandToInput<>() does NOT inherit from GameObjectCommand");

	std::unique_ptr<BindedCommand> newBindedCommand = std::make_unique<BindedCommand>();
	newBindedCommand->button = button;

	std::unique_ptr<T> tempCommandUPtr = std::make_unique<T>(gameObject);
	T* tempRawPointer = tempCommandUPtr.get();
	newBindedCommand->command = std::move(tempCommandUPtr);

	newBindedCommand->interactionState = interactionState;
	newBindedCommand->controllerIndex = controllerIndex;
	m_BindedCommands.emplace_back(std::move(newBindedCommand));
	return tempRawPointer;
}

template<typename T>
inline void Twengine::InputManager::RemoveBindedCommand(int controllerIndex)
{
	for (const auto& command : m_BindedCommands)
	{
		if (command->controllerIndex != controllerIndex) continue;
		T* tempCommand = dynamic_cast<T*>(command->command.get());
		if (tempCommand != nullptr) // If This Goes Through, This Is The Command That Should Be Removed
		{
			m_BindedCommands.erase(std::remove(m_BindedCommands.begin(), m_BindedCommands.end(), command), m_BindedCommands.end());
		}
	}	
}
