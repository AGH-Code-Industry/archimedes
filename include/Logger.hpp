#pragma once

#include "Logger.h"
#include <filesystem>

namespace arch {

template <typename... Args>
void Logger::_log_impl(LogLevel level, const std::source_location loc, spdlog::format_string_t<Args...> fmt, Args &&...args) {
	auto filepath = relative(loc.file_name(), std::filesystem::current_path() / "..");

	s_logger->log(
		spdlog::source_loc(filepath.string().c_str(), loc.line(), loc.function_name()),
		(spdlog::level::level_enum)(int)level,
		fmt,
		std::forward<Args>(args)...
	);
}


namespace _details {

template<typename... Args>
struct UniversalLogger {
	UniversalLogger(LogLevel level, spdlog::format_string_t<Args...> fmt, Args&&... args, std::source_location loc = std::source_location::current()) {
		Logger::_log_impl(level, loc, fmt, std::forward<Args>(args)...);
	}
};
template<typename... Args>
UniversalLogger(LogLevel level, spdlog::format_string_t<Args...>&& fmt, Args&&... args) -> UniversalLogger<Args...>;

template<LogLevel level, typename... Args>
    struct LeveledLogger {
	LeveledLogger(spdlog::format_string_t<Args...>&& fmt, Args&&... args, std::source_location loc = std::source_location::current()) {
		Logger::_log_impl(level, loc, fmt, std::forward<Args>(args)...);
	}
};
template<LogLevel level, typename... Args>
LeveledLogger(spdlog::format_string_t<Args...> fmt, Args&&... args) -> LeveledLogger<level, Args...>;

}

}