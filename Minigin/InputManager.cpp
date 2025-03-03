#include <SDL.h>
#include "InputManager.h"
#include "backends/imgui_impl_sdl2.h"

#include <iostream>

bool InputManager::ProcessInput()
{
	CopyMemory(&m_PreviousState, &m_CurrentState, sizeof(XINPUT_STATE));
	ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
	XInputGetState(0, &m_CurrentState);

	auto buttonChanges = m_CurrentState.Gamepad.wButtons ^ m_PreviousState.Gamepad.wButtons;
	m_ButtonsPressedThisFrame = buttonChanges & m_CurrentState.Gamepad.wButtons;
	m_ButtonsReleasedThisFrame = buttonChanges & (~m_CurrentState.Gamepad.wButtons);

	if (IsDownThisFrame(XINPUT_GAMEPAD_A))
	{
		std::cout << "A DOWN" << std::endl;
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
bool InputManager::IsDownThisFrame(unsigned int button) const
{
	return m_ButtonsPressedThisFrame & button;
}
bool InputManager::IsUpThisFrame(unsigned int button) const
{
	return m_ButtonsReleasedThisFrame & button;
}
bool InputManager::IsPressed(unsigned int button) const
{
	return m_CurrentState.Gamepad.wButtons & button;
}
// DeadZone as a var as it'll have to be in setings of game. default -> 10-15%