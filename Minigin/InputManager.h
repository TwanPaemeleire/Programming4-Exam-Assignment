#pragma once
#include "Singleton.h"

#include <vector>
#include <memory>

class GameObject;
#include "Controller.h"
#include "GameObjectCommand.h"

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
	command* BindCommandToInput(unsigned int button ,InteractionStates interactionState, GameObject* gameObject, int controllerIndex);

private:
	void HandleControllerInput();
	bool HandleKeyBoardInput();

	std::vector<std::unique_ptr<Controller>> m_Controllers;
	std::vector<std::unique_ptr<BindedCommand>> m_BindedCommands;
};

template<typename command>
inline command* InputManager::BindCommandToInput(unsigned int button, InteractionStates interactionState, GameObject* gameObject, int controllerIndex)
{
	static_assert(std::is_base_of<GameObjectCommand, command>::value, "Type passed to BindCommandToInput<>() does NOT inherit from Command");

	std::unique_ptr<BindedCommand> newBindedCommand = std::make_unique<BindedCommand>();
	newBindedCommand->button = button;

	std::unique_ptr<command> tempCommandUPtr = std::make_unique<command>(gameObject);
	command* tempRawPointer = tempCommandUPtr.get();
	newBindedCommand->command = std::move(tempCommandUPtr);

	newBindedCommand->interactionState = interactionState;
	newBindedCommand->controllerIndex = controllerIndex;
	m_BindedCommands.emplace_back(std::move(newBindedCommand));
	return tempRawPointer;
}
