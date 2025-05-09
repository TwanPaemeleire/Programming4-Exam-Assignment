#include <SDL.h>
#include "InputManager.h"
#include "backends/imgui_impl_sdl2.h"
#include <iostream>

void Twengine::InputManager::Init()
{
	for (int controllerIdx{ 0 }; controllerIdx < 4; ++controllerIdx)
	{
		m_Controllers.emplace_back(std::make_unique<Controller>(controllerIdx));
	}
}

bool Twengine::InputManager::ProcessInput()
{
	HandleControllerInput();
	return HandleKeyBoardInput();
}

void Twengine::InputManager::HandleControllerInput()
{
	for (const auto& controller : m_Controllers)
	{
		controller->ProcessInput(); // Process input for every controller
		int controllerIndex = controller->GetControllerIndex();

		// JoyStick input
		float leftX = controller->GetLeftStickX();
		float leftY = controller->GetLeftStickY();

		for (const auto& joystickCommand : m_BindedJoystickCommands)
		{
			if (joystickCommand->controllerIndex == controllerIndex)
			{
				joystickCommand->command->Execute(leftX, leftY);
			}
		}

		for (const auto& command : m_BindedCommands)
		{
			if (command->controllerIndex == -1) // If index is -1 then this command is binded to the keyboard
				continue;

			if (command->controllerIndex == controllerIndex) // Check if command is binded to this controller
			{
				bool executeCommand = false;

				switch (command->interactionState) // Check if correct InteractionState is achieved
				{
				case InteractionStates::down:
					executeCommand = controller->IsDownThisFrame(command->button);
					break;
				case InteractionStates::up:
					executeCommand = controller->IsUpThisFrame(command->button);
					break;
				case InteractionStates::pressed:
					executeCommand = controller->IsPressed(command->button);
					break;
				}

				if (executeCommand) // Execute the command if interactionState was achieved
				{
					command->command->Execute();
				}

			}
		}
	}
}

bool Twengine::InputManager::HandleKeyBoardInput()
{	
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT)
		{
			return false;
		}

		if (e.type == SDL_KEYUP || e.type == SDL_KEYDOWN)
		{
			for (const auto& command : m_BindedCommands)
			{
				if (command->controllerIndex != -1) // If index is not -1 then this command is binded to a controller
					continue;
				if (e.key.keysym.sym != static_cast<SDL_Keycode>(command->button)) // Check if command key is the same as the key we're currently checking
					continue;

				switch (command->interactionState)
				{
				case InteractionStates::down:
					if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
					{
						command->command->Execute();
					}
					break;

				case InteractionStates::up:
					if (e.type == SDL_KEYUP)
					{
						command->command->Execute();
					}
					break;

				default:
					break;
				}
			}
		}
		ImGui_ImplSDL2_ProcessEvent(&e);
	}


	const Uint8* keyStates = SDL_GetKeyboardState(NULL);

	for (const auto& command : m_BindedCommands)
	{
		if (command->controllerIndex != -1) continue;  // If index is not -1 then this command is binded to a controller
		if (command->interactionState != InteractionStates::pressed) continue; // Check if this command has the "Pressed" InteractionState

		if (keyStates[SDL_GetScancodeFromKey(command->button)]) // Check if command key is currently being pressed down
		{
			command->command->Execute();
		}
	}
	return true;
}
