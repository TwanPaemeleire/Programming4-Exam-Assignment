#pragma once
#include "Singleton.h"

#include <vector>
#include <memory>

class Command;
#include "Controller.h"

class InputManager final : public Singleton<InputManager>
{
public:
	void Init();
	bool ProcessInput();

private:

	std::vector<std::unique_ptr<Controller>> m_Controllers;

	//std::vector<std::unique_ptr<Command>> m_Commands;
};