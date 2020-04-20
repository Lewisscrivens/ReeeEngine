#pragma once
#include "Globals.h"
#include "spdlog/spdlog.h"
#include "spdlog/fmt/ostr.h"

/* Define logging macros for logging to the engine or app logging system. */
#define REEE_LOG(LogType, ...) ReeeLog::CreateLog(LogType, __VA_ARGS__);

namespace ReeeEngine
{
	/* Define logging enum... */
	enum ELogType
	{
		Log,
		Warning,
		Error,
		//....
	};

	/* Logging class to hold application and engine logging extended from the spdlog library. */
	class REEE_API ReeeLog
	{	
	public:

		/* Initialize the loggers for both the engine and application. */
		static void InitaliseLogging();

		/* Logging function for performing a log message to either the engine or application. */
		template<typename... Args>
		static void CreateLog(ELogType logType, fmt::basic_string_view<char> message, const Args &... args)
		{
			switch (logType)
			{
			case Log:
				ReeeLog::GetEngineLogger()->trace(message, args...);
				break;
			case Warning:
				ReeeLog::GetEngineLogger()->warn(message, args...);
				break;
			case Error:
				ReeeLog::GetEngineLogger()->error(message, args...);
				break;
			}
		}

		/* Getters for different types of loggers. */
		static Pointer<spdlog::logger>& GetEngineLogger() { return engineLogger; }

	private:

		/* Pointers to the engine logger and the application logger. */
		static Pointer<spdlog::logger> engineLogger;
	};
}

