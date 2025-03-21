#include "Controller.h"

#define WIN32_LEAN_AND_MEAN
#include "Windows.h"
#include "Xinput.h"

class Twengine::Controller::XInputImpl
{
public:
	XInputImpl(int controllerIndex) :m_ControllerIndex{ controllerIndex } {};

	void ProcessInput();
	bool IsDownThisFrame(unsigned int button) const;
	bool IsUpThisFrame(unsigned int button) const;
	bool IsPressed(unsigned int button) const;

	int GetControllerIndex() const;

private:
	XINPUT_STATE m_PreviousState{};
	XINPUT_STATE m_CurrentState{};
	WORD m_ButtonsPressedThisFrame{};
	WORD m_ButtonsReleasedThisFrame{};

	int m_ControllerIndex{};
};

Twengine::Controller::Controller(int controllerIndex)
	:m_pImpl{new XInputImpl(controllerIndex)}
{

}

Twengine::Controller::~Controller()
{
	delete m_pImpl;
}

void Twengine::Controller::ProcessInput()
{
	m_pImpl->ProcessInput();
}

bool Twengine::Controller::IsDownThisFrame(unsigned int button) const
{
	return m_pImpl->IsDownThisFrame(button);
}
bool Twengine::Controller::IsUpThisFrame(unsigned int button) const
{
	return m_pImpl->IsUpThisFrame(button);
}
bool Twengine::Controller::IsPressed(unsigned int button) const
{
	return m_pImpl->IsPressed(button);
}

int Twengine::Controller::GetControllerIndex() const
{
	return m_pImpl->GetControllerIndex();;
}

void Twengine::Controller::XInputImpl::ProcessInput()
{
	CopyMemory(&m_PreviousState, &m_CurrentState, sizeof(XINPUT_STATE));
	ZeroMemory(&m_CurrentState, sizeof(XINPUT_STATE));
	XInputGetState(m_ControllerIndex, &m_CurrentState);

	auto buttonChanges = m_CurrentState.Gamepad.wButtons ^ m_PreviousState.Gamepad.wButtons;
	m_ButtonsPressedThisFrame = buttonChanges & m_CurrentState.Gamepad.wButtons;
	m_ButtonsReleasedThisFrame = buttonChanges & (~m_CurrentState.Gamepad.wButtons);
}

bool Twengine::Controller::XInputImpl::IsDownThisFrame(unsigned int button) const
{
	return m_ButtonsPressedThisFrame & button;
}

bool Twengine::Controller::XInputImpl::IsUpThisFrame(unsigned int button) const
{
	return m_ButtonsReleasedThisFrame & button;
}

bool Twengine::Controller::XInputImpl::IsPressed(unsigned int button) const
{
	return m_CurrentState.Gamepad.wButtons & button;
}

int Twengine::Controller::XInputImpl::GetControllerIndex() const
{
	return m_ControllerIndex;
}
