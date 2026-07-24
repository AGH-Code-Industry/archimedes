#pragma once

#include <spdlog/spdlog.h>

namespace arch {

class Engine;

}

namespace arch::log {

/// @brief Logging level for Logger to indicate how important the message is
enum class Level {
	none = -1,
	/// @brief Detailed trace information
	trace = spdlog::level::trace,
	/// @brief Debugging information
	debug = spdlog::level::debug,
	/// @brief General information
	info = spdlog::level::info,
	/// @brief Information representing a minor problem that has occurred
	warn = spdlog::level::warn,
	/// @brief Information representing a serious problem that has occurred
	error = spdlog::level::err,
	/// @brief Information representing a critical problem that has occurred
	crit = spdlog::level::critical,
};

/// @brief Sets global logger level
extern void setLevel(Level level);

namespace _details {

/// @brief Logger implementation
/// @param level - log level
/// @param stacktraceSkip - stacktrace entries to skip, including logImpl itself
/// @param fmt - format string
/// @param args... - arguments to format
template<typename... Args>
void logImpl(Level level, const u32 stacktraceSkip, std::format_string<Args...> fmt, Args&&... args);

/// @brief Type of log::logger
struct UniversalLogger {
	/// @brief Logging operator
	/// @param level - log level
	/// @param fmt - format string
	/// @param args... - arguments to format
	template<class... Args>
	static void operator()(Level level, std::format_string<Args...> fmt, Args&&... args);
};

/// @brief Type of leveled loggers
/// @tparam L - logger level
template<Level L>
struct LeveledLogger {
	/// @brief Logging operator
	/// @param fmt - format string
	/// @param args... - arguments to format
	template<class... Args>
	static void operator()(std::format_string<Args...> fmt, Args&&... args);

	/// @brief Conversion operator, enables use of log::<level> as enum values, instead of log::Level::<level>
	consteval operator Level() const noexcept;
};

class LoggerSingleton {
public:
	/// @brief Initializes global logger
	/// @param name - logger name
	/// @param file - whether to log to a file
	static void init(const std::string& name, bool file);

private:
	template<class... Args>
	friend void logImpl(Level, spdlog::format_string_t<Args...>, Args&&...);
	friend class ::arch::Engine;
	friend void ::arch::log::setLevel(Level);

	static std::shared_ptr<spdlog::logger> _logger;
};

} // namespace _details

/// @brief Universal logger, requires log::Level to work
extern _details::UniversalLogger logger;

/// @brief Logger for detailed trace information
extern _details::LeveledLogger<Level::trace> trace;
/// @brief Logger for debugging information
extern _details::LeveledLogger<Level::debug> debug;
/// @brief Logger for general information
extern _details::LeveledLogger<Level::info> info;
/// @brief Logger for information representing a minor problem that has occurred
extern _details::LeveledLogger<Level::warn> warn;
/// @brief Logger for information representing a serious problem that has occurred
extern _details::LeveledLogger<Level::error> error;
/// @brief Logger for information representing a critical problem that has occurred
extern _details::LeveledLogger<Level::crit> crit;

} // namespace arch::log

#include "Logger.hpp"
