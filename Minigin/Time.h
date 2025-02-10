#pragma once
#include "Singleton.h"

class Time final : public Singleton<Time>
{
public:
	float deltaTime{};
private:
	friend class Singleton<Time>;
	Time() = default;
};

