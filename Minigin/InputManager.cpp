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
		controller->ProcessInput(); // Process Input For Every Controller
		int controllerIndex = controller->GetControllerIndex();

		// JoyStick Input
		float leftX = controller->GetLeftStickX();
		float leftY = controller->GetLeftStickY();

		//if (fabs(leftX) >= 0.1f || fabs(leftY) >= 0.1f)
		//{
			for (const auto& joystickCommand : m_BindedJoystickCommands)
			{
				if (joystickCommand->controllerIndex == controllerIndex)
				{
					joystickCommand->command->Execute(leftX, leftY);
				}
			}
		//}


		for (const auto& command : m_BindedCommands)
		{
			if (command->controllerIndex == -1) // If Index Is -1 Then This Command Is Binded To The Keyboard
				continue;

			if (command->controllerIndex == controllerIndex) // Check If Command Is Binded To This Controller
			{
				bool executeCommand = false;

				switch (command->interactionState) // Check If Correct InteractionState Is Achieved
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

				if (executeCommand) // Execute The Command If InteractionState Was Achieved
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
				if (command->controllerIndex != -1) // If Index Is Not -1 Then This Command Is Binded To A Controller
					continue;
				if (e.key.keysym.sym != static_cast<SDL_Keycode>(command->button)) // Check If Command Key Is The Same As The Key We're Currently Checking
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
		if (command->controllerIndex != -1) continue;  // If Index Is Not -1 Then This Command Is Binded To A Controller
		if (command->interactionState != InteractionStates::pressed) continue; // Check If This Command Has The "Pressed" Interaction State

		if (keyStates[SDL_GetScancodeFromKey(command->button)]) // Check If Command Key Is Currently Being Pressed Down
		{
			command->command->Execute();
		}
	}
	return true;
}
