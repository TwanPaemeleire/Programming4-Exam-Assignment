#pragma once
#include "Singleton.h"

namespace Twengine
{
	class Time final : public Singleton<Time>
	{
	public:
		float deltaTime{};
	private:
		friend class Singleton<Time>;
		Time() = default;
	};
}