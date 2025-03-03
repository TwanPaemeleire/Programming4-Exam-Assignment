#include <SDL.h>
#include "InputManager.h"
#include "backends/imgui_impl_sdl2.h"

void InputManager::Init()
{
	for (int controllerIdx{ 0 }; controllerIdx < 4; ++controllerIdx)
	{
		m_Controllers.emplace_back(std::make_unique<Controller>(controllerIdx));
	}
}

bool InputManager::ProcessInput()
{
	HandleControllerInput();
	return HandleKeyBoardInput();
}

void InputManager::HandleControllerInput()
{
	for (const auto& controller : m_Controllers)
	{
		controller->ProcessInput(); // Process Input For Every Controller
		int controllerIndex = controller->GetControllerIndex();

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

bool InputManager::HandleKeyBoardInput()
{	
	SDL_Event e;
	while (SDL_PollEvent(&e))
	{
		if (e.type == SDL_QUIT)
		{
			return false;
		}
	
		for (const auto& command : m_BindedCommands)
		{
			if (command->controllerIndex != -1) // If Index Is Not -1 Then This Command Is Binded To A Controller
				continue;
			if (e.key.keysym.sym != static_cast<SDL_Keycode>(command->button)) // Check If Command Key Is The Same As The Key We're Currently Checking
				continue;
	
			bool executeCommand = false;
	
			switch (command->interactionState)  // Check If Correct InteractionState Is Achieved
			{
			case InteractionStates::down:
				if (e.type == SDL_KEYDOWN && e.key.repeat == 0)
				{
					executeCommand = true;
				}
				break;
			case InteractionStates::up:
				if (e.type == SDL_KEYUP)
				{
					executeCommand = true;
				}
				break;
			case InteractionStates::pressed:
				if (e.type == SDL_KEYDOWN)
				{
					executeCommand = true;
				}
				break;
			}
	
			if (executeCommand) // Execute The Command If InteractionState Was Achieved
			{
				command->command->Execute();
			}
		}
		ImGui_ImplSDL2_ProcessEvent(&e);
	}
	return true;
}
