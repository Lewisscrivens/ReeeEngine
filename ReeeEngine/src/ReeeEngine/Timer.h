#pragma once
#include <chrono>
#include "Globals.h"

namespace ReeeEngine
{
	/* Timer class to keep track of time that the application has been opened. */
	class REEE_API Timer
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