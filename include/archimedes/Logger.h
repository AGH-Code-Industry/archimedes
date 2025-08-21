#pragma once

#include <source_location>

#include <spdlog/spdlog.h>

namespace arch {

/// @brief Logging level for Logger to indicate how important the message is.
///
enum class LogLevel {
	none = -1,
	/// @brief Detailed trace information ().
	trace,
	/// @brief Debugging information.
	debug,
	/// @brief General information.
	info,
	/// @brief Information representing a minor problem that has occurred.
	warn,
	/// @brief Information representing a serious problem that has occurred.
	error,
	/// @brief Information representing a critical problem that has occurred.s
	critical,
};

/// @brief Implementation details.
namespace _details {

template<typename... Args>
struct UniversalLogger;

template<LogLevel Level, typename... Args>
struct LeveledLogger;

} // namespace _details

/// @brief Main static Logger class for logging messages.
///
class Logger {
	template<typename... Args>
	friend struct _details::UniversalLogger;

	template<LogLevel Level, typename... Args>
	friend struct _details::LeveledLogger;

public:

	/// @brief Initializes the logger.
	/// @param logLevel minimum level of log messages to be logged. (All messages with lower level will be ignored)
	/// @param name name of the logger. Used for identifying the logger.
	static void init(LogLevel logLevel, const std::string& name = "Logger");

	/// @brief Universal logger for logging messages using runtime @enum LogLevel.
	template<typename... Args>
	using log = _details::UniversalLogger<Args...>; // NOLINT(*-identifier-naming)

	/// @brief Fast way of logging messages at @enum LogLevel::trace.
	template<typename... Args>
	using trace = _details::LeveledLogger<LogLevel::trace, Args...>; // NOLINT(*-identifier-naming)

	/// @brief Fast way of logging messages at @enum LogLevel::debug.
	template<typename... Args>
	using debug = _details::LeveledLogger<LogLevel::debug, Args...>; // NOLINT(*-identifier-naming)

	/// @brief Fast way of logging messages at @enum LogLevel::info.
	template<typename... Args>
	using info = _details::LeveledLogger<LogLevel::info, Args...>; // NOLINT(*-identifier-naming)

	/// @brief Fast way of logging messages at \enum LogLevel::warn.
	template<typename... Args>
	using warn = _details::LeveledLogger<LogLevel::warn, Args...>; // NOLINT(*-identifier-naming)

	/// @brief Fast way of logging messages at \enum LogLevel::error.
	template<typename... Args>
	using error = _details::LeveledLogger<LogLevel::error, Args...>; // NOLINT(*-identifier-naming)

	/// @brief Fast way of logging messages at \enum LogLevel::critical.
	template<typename... Args>
	using critical = _details::LeveledLogger<LogLevel::critical, Args...>; // NOLINT(*-identifier-naming)

private:

	template<typename... Args>
	static void _logImpl(
		LogLevel level,
		std::source_location loc,
		spdlog::format_string_t<Args...> fmt,
		Args&&... args
	);

	static std::shared_ptr<spdlog::logger> s_logger;
};

} // namespace arch

#include "Logger.hpp"
