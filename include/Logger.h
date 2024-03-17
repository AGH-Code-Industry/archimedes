#pragma once
#include <spdlog/spdlog.h>

#include <source_location>

namespace arch {

/// @brief Logging level for Logge to indicate how important the message is.
///
enum class LogLevel
{
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

template <typename... Args>
struct UniversalLogger;

template <LogLevel Level, typename... Args>
struct LeveledLogger;

}

/// @brief Main static Logger class for logging messages.
///
class Logger {
	template <typename... Args>
	friend struct _details::UniversalLogger;

	template <LogLevel Level, typename... Args>
	friend struct _details::LeveledLogger;
public:

	/// @brief Initializes the logger.
	/// @param logLevel minimum level of log messages to be logged. (All messages with lower level will be ignored)
	/// @param name name of the logger. Used for identifying the logger.
	static void init(LogLevel logLevel, const std::string& name = "Logger");

	/// @brief Universal logger for logging messages using runtime @enum LogLevel.
	template <typename... Args>
	using log = _details::UniversalLogger<Args...>;

	/// @brief Fast way of loggin messages at @enum LogLevel::trace.
	template <typename... Args>
	using trace = _details::LeveledLogger<LogLevel::trace, Args...>;

	/// @brief Fast way of loggin messages at @enum LogLevel::debug.
	template <typename... Args>
	using debug = _details::LeveledLogger<LogLevel::debug, Args...>;

	/// @brief Fast way of loggin messages at @enum LogLevel::info.
	template<typename... Args>
	using info = _details::LeveledLogger<LogLevel::info, Args...>;

	/// @brief Fast way of loggin messages at \enum LogLevel::warn.
	template<typename... Args>
	using warn = _details::LeveledLogger<LogLevel::warn, Args...>;

	/// @brief Fast way of loggin messages at \enum LogLevel::error.
	template<typename... Args>
	using error = _details::LeveledLogger<LogLevel::error, Args...>;

	/// @brief Fast way of loggin messages at \enum LogLevel::critical.
	template<typename... Args>
	using critical = _details::LeveledLogger<LogLevel::critical, Args...>;

private:

	template <typename... Args>
	static void _log_impl(LogLevel level, std::source_location loc, spdlog::format_string_t<Args...> fmt, Args&&... args);

	static std::shared_ptr<spdlog::logger> s_logger;
};

}

#include "Logger.hpp"