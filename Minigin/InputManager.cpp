#include <SDL.h>
#include "InputManager.h"
#include "backends/imgui_impl_sdl2.h"

#include <iostream>

void InputManager::Init()
{
	for (int controllerIdx{ 0 }; controllerIdx < 2; ++controllerIdx)
	{
		m_Controllers.emplace_back(std::make_unique<Controller>(controllerIdx));
	}
}

bool InputManager::ProcessInput()
{
	// Make All The Controllers Process Input
	for (const auto& controller : m_Controllers)
	{
		controller->ProcessInput();
		bool isDown = controller->IsDownThisFrame(XINPUT_GAMEPAD_A);
		if (isDown) std::cout << "A DOWN FOR CONTROLLER: " << controller->m_ControllerIndex << std::endl;
	}


	SDL_Event e;
	while (SDL_PollEvent(&e)) 
	{
		if (e.type == SDL_QUIT) {
			return false;
		}
		if (e.type == SDL_KEYDOWN) {
			
		}
		if (e.type == SDL_MOUSEBUTTONDOWN) {
			
		}
		// etc...

		ImGui_ImplSDL2_ProcessEvent(&e);
	}

	return true;
}