#pragma once
#include "Singleton.h"

#include <vector>
#include <memory>

class GameObject;
#include "Controller.h"
#include "GameObjectCommand.h"
#include "JoystickCommand.h"

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
		int controllerIndex{}; // An index of -1 will mean that it's binded to keyboard
		unsigned int button{};
	};

	struct BindedJoystickCommand
	{
		std::unique_ptr<JoystickCommand> command{};
		InteractionStates interactionState{};
		int controllerIndex{};
	};

	class InputManager final : public Singleton<InputManager>
	{
	public:
		void Init();
		bool ProcessInput();

		template <typename command>
		command* BindCommandToInput(unsigned int button, InteractionStates interactionState, GameObject* gameObject, int controllerIndex);
		template <typename command>
		command* BindJoystickCommandToInput(InteractionStates interactionState, GameObject* gameObject, int controllerIndex);
		template <typename command>
		void RemoveBindedCommand(int controllerIndex);

	private:
		void HandleControllerInput();
		bool HandleKeyBoardInput();

		std::vector<std::unique_ptr<Controller>> m_Controllers;
		std::vector<std::unique_ptr<BindedCommand>> m_BindedCommands;
		std::vector<std::unique_ptr<BindedJoystickCommand>> m_BindedJoystickCommands;
	};
}



template<typename T>
inline T* Twengine::InputManager::BindCommandToInput(unsigned int button, Twengine::InteractionStates interactionState, GameObject* gameObject, int controllerIndex)
{
	static_assert(std::is_base_of<GameObjectCommand, T>::value, "Type passed to BindCommandToInput<>() does NOT inherit from GameObjectCommand");

	std::unique_ptr<BindedCommand> newBindedCommand = std::make_unique<BindedCommand>();
	newBindedCommand->button = button;

	std::unique_ptr<T> tempCommandUptr = std::make_unique<T>(gameObject);
	T* tempRawPointer = tempCommandUptr.get();
	newBindedCommand->command = std::move(tempCommandUptr);

	newBindedCommand->interactionState = interactionState;
	newBindedCommand->controllerIndex = controllerIndex;
	m_BindedCommands.emplace_back(std::move(newBindedCommand));
	return tempRawPointer;
}

template<typename command>
inline command* Twengine::InputManager::BindJoystickCommandToInput(InteractionStates interactionState, GameObject* gameObject, int controllerIndex)
{
	static_assert(std::is_base_of<JoystickCommand, command>::value, "Type passed to BindJoyStickCommandToInput<>() does NOT inherit from JoystickCommand");

	std::unique_ptr<BindedJoystickCommand> newBindedCommand = std::make_unique<BindedJoystickCommand>();
	std::unique_ptr<command> tempCommandUptr = std::make_unique<command>(gameObject);
	command* tempRawPointer = tempCommandUptr.get();
	newBindedCommand->command = std::move(tempCommandUptr);

	newBindedCommand->interactionState = interactionState;
	newBindedCommand->controllerIndex = controllerIndex;
	m_BindedJoystickCommands.emplace_back(std::move(newBindedCommand));
	return tempRawPointer;
}

template<typename T>
inline void Twengine::InputManager::RemoveBindedCommand(int controllerIndex)
{
	static_assert(std::is_base_of<GameObjectCommand, T>::value, "Type passed to RemoveBindedCommand<>() does NOT inherit from GameObjectCommand");
	for (const auto& command : m_BindedCommands)
	{
		if (command->controllerIndex != controllerIndex) continue;
		T* tempCommand = dynamic_cast<T*>(command->command.get());
		if (tempCommand != nullptr) // If this goes through, this is the command that should be removed
		{
			m_BindedCommands.erase(std::remove(m_BindedCommands.begin(), m_BindedCommands.end(), command), m_BindedCommands.end());
		}
	}	
}
