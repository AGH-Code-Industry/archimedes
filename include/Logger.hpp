#pragma once

#include "Logger.h"
#include <filesystem>

namespace arch {

template <typename... Args>
void Logger::_log_impl(LogLevel level, const std::source_location loc, spdlog::format_string_t<Args...> fmt, Args&&... args) {
	// Get the file path relative to the current working directory
	auto filepath = relative(loc.file_name(), std::filesystem::current_path() / "..");

	// Log the message
	s_logger->log(
		spdlog::source_loc(filepath.string().c_str(), loc.line(), loc.function_name()),
		(spdlog::level::level_enum)level,
		fmt,
		std::forward<Args>(args)...
	);
}


namespace _details {

template<typename... Args>
struct UniversalLogger {
	/// @brief Universal logger for logging messages using runtime @enum LogLevel.
	/// @param level level of the log message.
	/// @param fmt format string for the log message.
	/// @param args arguments to be formatted into the log message.
	/// @param loc source location of the log message. (default: current source location)
	UniversalLogger(LogLevel level, spdlog::format_string_t<Args...> fmt, Args&&... args, std::source_location loc = std::source_location::current()) {
		// Pass all arguments to the logger
		Logger::_log_impl(level, loc, fmt, std::forward<Args>(args)...);
	}
};
// Deduction guide to allow for template argument deduction without requiring the user to specify the template arguments
template<typename... Args>
UniversalLogger(LogLevel level, spdlog::format_string_t<Args...>&& fmt, Args&&... args) -> UniversalLogger<Args...>;


template<LogLevel level, typename... Args>
struct LeveledLogger {
	/// @param fmt format string for the log message.
	/// @param args arguments to be formatted into the log message.
	/// @param loc source location of the log message. (default: current source location)
	LeveledLogger(spdlog::format_string_t<Args...>&& fmt, Args&&... args, std::source_location loc = std::source_location::current()) {
		// Pass all arguments to the logger
		Logger::_log_impl(level, loc, fmt, std::forward<Args>(args)...);
	}
};
// Deduction guide to allow for template argument deduction without requiring the user to specify the template arguments
template<LogLevel level, typename... Args>
LeveledLogger(spdlog::format_string_t<Args...> fmt, Args&&... args) -> LeveledLogger<level, Args...>;

}

}