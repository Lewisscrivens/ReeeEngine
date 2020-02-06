#pragma once
#include <chrono>

namespace ReeeEngine
{
	/* Timer class to keep track of time that the application has been opened. */
	class Timer
	{
	public:

		/* Constructor. */
		Timer() noexcept;

		/* TImer functions. */
		float GetDeltaTime() noexcept;// Returns current frame delta time.
		float GetTime() const noexcept;// Returns current app time as a float.

	private:

		/* Time since app initialized timer class. */
		std::chrono::steady_clock::time_point time;

	};
}