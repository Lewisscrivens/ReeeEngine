#pragma once
#include "../Globals.h"
#include "../ReeeLog.h"
#include <chrono>
#include <thread>

/* Define profiling macros. */
#ifdef PROFILING
	#define PROFILE_SCOPE(name) ReeeEngine::DebugTimer debugTimer(name);
#else
	#define PROFILE_BEGIN(name, file)
	#define PROFILE_END()
	#define PROFILE_SCOPE(name)
#endif

namespace ReeeEngine
{
	/* Scope based timer class for timing its time existing within a given scope. */
	class REEE_API DebugTimer
	{
	private:

		/* The timer. */
		std::chrono::time_point<std::chrono::high_resolution_clock> startPoint;

		/* Timer handle name. */
		const char* timerName;

	public:

		/* Timer start setup. */
		DebugTimer(const char* name) : timerName(name) { startPoint = std::chrono::high_resolution_clock::now(); }

		/* Destructor end timer scope. */
		~DebugTimer()
		{
			auto endPoint = std::chrono::high_resolution_clock::now();
			long long startTime = std::chrono::time_point_cast<std::chrono::milliseconds>(startPoint).time_since_epoch().count();
			long long endTime = std::chrono::time_point_cast<std::chrono::milliseconds>(endPoint).time_since_epoch().count();
			long long durationMS = endTime - startTime;

			// Log time taken.
			REEE_LOG(Log, "[TIMER][{0}] {1} ms.", timerName, durationMS);
		}
	};
}
