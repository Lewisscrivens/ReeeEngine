#include "Timer.h"

namespace ReeeEngine
{
	Timer::Timer() noexcept
	{
		Reset();
	}

	void Timer::Reset()
	{
		time = std::chrono::steady_clock::now();
	}

	float Timer::GetDeltaTime() noexcept
	{
		const auto lastTime = time;
		time = std::chrono::steady_clock::now();
		const std::chrono::duration<float> frameTime = time - lastTime;
		return frameTime.count();
	}

	float Timer::GetTime() const noexcept
	{
		return std::chrono::duration<float>(std::chrono::steady_clock::now() - time).count();
	}
}
