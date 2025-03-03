#pragma once
#include "Singleton.h"

#include <Windows.h>

#include "Xinput.h"

#include <vector>
#include <memory>

class Command;

class InputManager final : public Singleton<InputManager>
{
public:
	bool ProcessInput();

	bool IsDownThisFrame(unsigned int button) const;
	bool IsUpThisFrame(unsigned int button) const;
	bool IsPressed(unsigned int button) const;

private:
	XINPUT_STATE m_PreviousState{};
	XINPUT_STATE m_CurrentState{};
	WORD m_ButtonsPressedThisFrame{};
	WORD m_ButtonsReleasedThisFrame{};
	int m_ControllerIndex{};

	//std::vector<std::unique_ptr<Command>> m_Commands;
};