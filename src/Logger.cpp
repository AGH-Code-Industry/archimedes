#include "Logger.h"

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

namespace arch {

std::shared_ptr<spdlog::logger> Logger::s_logger = nullptr;

void Logger::init(LogLevel logLevel, const std::string& name) {
	// Check if the logger is already initialized
	if (s_logger) {
		warn("Logger is already initialized");
		return;
	}

	std::vector<spdlog::sink_ptr> logSinks;

	{
		// Add a console sink
		auto sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
		sink->set_pattern("%^[%T] [%l] [%@]: %v%$");

		logSinks.emplace_back(sink);
	}

#ifndef ARCH_NO_LOG_FILE
	{
		// Add a file sink
		char logPath[256];
		std::snprintf(logPath, sizeof(logPath), "Logs/%s-%lld.log", name.c_str(), std::time(nullptr));

		auto sink = std::make_shared<spdlog::sinks::basic_file_sink_mt>(logPath, true);
		sink->set_pattern("[%T] [%l] [%@]: %v");
		logSinks.emplace_back(sink);
	}
#endif

	// Create logger
	s_logger = std::make_unique<spdlog::logger>(name, logSinks.begin(), logSinks.end());
	register_logger(s_logger);

	// SEt the log level
	s_logger->set_level((spdlog::level::level_enum)logLevel);
	s_logger->flush_on(spdlog::level::debug);
}

} // namespace arch
