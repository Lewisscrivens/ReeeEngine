#include "ReeeLog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

namespace ReeeEngine
{
	// Initalise the engine logger.
	Pointer<spdlog::logger> ReeeLog::engineLogger;

	void ReeeEngine::ReeeLog::InitaliseLogging()
	{
		// Register two different log sinks, one for a file and one to update the console based logging...
		std::vector<spdlog::sink_ptr> logSinks;
		logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
		logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("../Log.log", true));// Place in core project folder.

		// Setup pattern to log messages in both the console and file versions of the engine logger.
		logSinks[0]->set_pattern("%^[%T] %n: %v%$");
		logSinks[1]->set_pattern("[%T] [%l] %n: %v");

		// Init the engine logger.
		engineLogger = CreatePointer<spdlog::logger>("ReeeEngine", begin(logSinks), end(logSinks));
		spdlog::register_logger(engineLogger);
		engineLogger->set_level(spdlog::level::trace);
		engineLogger->flush_on(spdlog::level::trace);
	}
}
