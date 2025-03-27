#pragma once
#include <memory>

namespace Twengine
{
	class Controller final
	{
	public:
		Controller(int controllerIndex);
		~Controller();
		Controller(const Controller& other) = delete;
		Controller(Controller&& other) = delete;
		Controller& operator=(const Controller& other) = delete;
		Controller& operator=(Controller&& other) = delete;

		void ProcessInput();
		bool IsDownThisFrame(unsigned int button) const;
		bool IsUpThisFrame(unsigned int button) const;
		bool IsPressed(unsigned int button) const;

		int GetControllerIndex() const;
		float GetLeftStickX() const;
		float GetLeftStickY() const;
	private:
		class XInputImpl;
		XInputImpl* m_pImpl;
	};
}