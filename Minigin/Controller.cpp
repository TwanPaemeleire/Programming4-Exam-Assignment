#include "Controller.h"
#include <iostream>

#include "Windows.h"
#include "Xinput.h"

class Controller::XInputImpl
{
public:
	XInputImpl(Controller* controller) :m_Controller{ controller } {};

	void ProcessInput();
	bool IsDownThisFrame(unsigned int button) const;
	bool IsUpThisFrame(unsigned int button) const;
	bool IsPressed(unsigned int button) const;

private:
	XINPUT_STATE m_PreviousState{};
	XINPUT_STATE m_CurrentState{};
	WORD m_ButtonsPressedThisFrame{};
	WORD m_ButtonsReleasedThisFrame{};

	Controller* m_Controller;
};

Controller::Controller(int controllerIndex)
	:m_ControllerIndex{controllerIndex}, m_Impl{new XInputImpl(this)}
{
	
}

Controller::~Controller()
{
	delete m_Impl;
}

void Controller::ProcessInput()
{
	m_Impl->ProcessInput();
}

bool Controller::IsDownThisFrame(unsigned int button) const
{
	return m_Impl->IsDownThisFrame(button);
}
bool Controller::IsUpThisFrame(unsigned int button) const
{
	return m_Impl->IsUpThisFrame(button);
}
bool Controller::IsPressed(unsigned int button) const
{
	return m_Impl->IsPressed(button);
}

int Controller::GetControllerIndex() const
{
	return m_ControllerIndex;
}

void Controller::XInputImpl::ProcessInput()
{
	CopyMemory(&m_PreviousState, &m_CurrentState, sizeof(XINPUT_STATE));
	ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
	XInputGetState(m_Controller->GetControllerIndex(), &m_CurrentState);

	auto buttonChanges = m_CurrentState.Gamepad.wButtons ^ m_PreviousState.Gamepad.wButtons;
	m_ButtonsPressedThisFrame = buttonChanges & m_CurrentState.Gamepad.wButtons;
	m_ButtonsReleasedThisFrame = buttonChanges & (~m_CurrentState.Gamepad.wButtons);
}

bool Controller::XInputImpl::IsDownThisFrame(unsigned int button) const
{
	return m_ButtonsPressedThisFrame & button;
}

bool Controller::XInputImpl::IsUpThisFrame(unsigned int button) const
{
	return m_ButtonsReleasedThisFrame & button;
}

bool Controller::XInputImpl::IsPressed(unsigned int button) const
{
	return m_CurrentState.Gamepad.wButtons & button;
}
