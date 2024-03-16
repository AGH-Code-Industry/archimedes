#pragma once
#include <spdlog/spdlog.h>

#include <format>
#include <source_location>

namespace arch {

enum class LogLevel
{
	none = -1,
	trace,
	debug,
	info,
	warn,
	error,
	critical,
};

namespace _details {

template <typename... Args>
struct UniversalLogger;

template <LogLevel Level, typename... Args>
struct LeveledLogger;

}

class Logger {
	template <typename... Args>
	friend struct _details::UniversalLogger;

	template <LogLevel Level, typename... Args>
	friend struct _details::LeveledLogger;
public:

	static void init(LogLevel logLevel, const std::string& name = "Logger");


	template <typename... Args>
	using log = _details::UniversalLogger<Args...>;

	template <typename... Args>
	using trace = _details::LeveledLogger<LogLevel::trace, Args...>;

	template <typename... Args>
	using debug = _details::LeveledLogger<LogLevel::debug, Args...>;

	template<typename... Args>
	using info = _details::LeveledLogger<LogLevel::info, Args...>;

	template<typename... Args>
	using warn = _details::LeveledLogger<LogLevel::warn, Args...>;

	template<typename... Args>
	using error = _details::LeveledLogger<LogLevel::error, Args...>;

	template<typename... Args>
	using critical = _details::LeveledLogger<LogLevel::critical, Args...>;

private:

	template <typename... Args>
	static void _log_impl(LogLevel level, std::source_location loc, spdlog::format_string_t<Args...> fmt, Args&&... args);

	static std::shared_ptr<spdlog::logger> s_logger;
};

}

#include "Logger.hpp"