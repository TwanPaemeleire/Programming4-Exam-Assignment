#pragma once
#include <string>
#include <functional>

namespace Twengine
{
	class Minigin
	{
	public:
		explicit Minigin();
		~Minigin();
		void Run(const std::function<void()>& load);

		Minigin(const Minigin& other) = delete;
		Minigin(Minigin&& other) = delete;
		Minigin& operator=(const Minigin& other) = delete;
		Minigin& operator=(Minigin&& other) = delete;

	private:
		const float m_FixedTimeStep{ 1.f / 60.f };
		const int m_MsPerFrame{ 1000 / 60 };
	};
}