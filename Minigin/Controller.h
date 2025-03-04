#pragma once

class Controller final
{
public:
	Controller(int controllerIndex);
	~Controller();

	void ProcessInput();
	bool IsDownThisFrame(unsigned int button) const;
	bool IsUpThisFrame(unsigned int button) const;
	bool IsPressed(unsigned int button) const;

	int GetControllerIndex() const;
private:
	int m_ControllerIndex;

	class XInputImpl;
	XInputImpl* m_pImpl;
};

