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

extern void setLevel(Level level);

namespace _details {

template<typename... Args>
void logImpl(Level level, std::format_string<Args...> fmt, Args&&... args);

struct UniversalLogger {
	template<class... Args>
	static void operator()(Level level, std::format_string<Args...> fmt, Args&&... args);
};

template<Level L>
struct LeveledLogger {
	template<class... Args>
	static void operator()(std::format_string<Args...> fmt, Args&&... args);

	consteval operator Level() const noexcept;
};

class LoggerSingleton {
public:
	static void init(const std::string& name, bool file);

private:
	template<class... Args>
	friend void logImpl(Level, spdlog::format_string_t<Args...>, Args&&...);
	friend class ::arch::Engine;
	friend void ::arch::log::setLevel(Level);

	static std::shared_ptr<spdlog::logger> _logger;
};

} // namespace _details

extern _details::UniversalLogger logger;

extern _details::LeveledLogger<Level::trace> trace;
extern _details::LeveledLogger<Level::debug> debug;
extern _details::LeveledLogger<Level::info> info;
extern _details::LeveledLogger<Level::warn> warn;
extern _details::LeveledLogger<Level::error> error;
extern _details::LeveledLogger<Level::crit> crit;

} // namespace arch::log

#include "Logger.hpp"
