#pragma once

#include <filesystem>
#include <stacktrace>

#include "Logger.h"
#include <archimedes/utils/ParseStacktrace.h>

namespace arch::log::_details {

template<class... Args>
void logImpl(Level level, const u32 stacktraceSkip, std::format_string<Args...> fmt, Args&&... args) {
	namespace fs = std::filesystem;

	// 0: this
	// 1: logger
	// 2: actual location
	// ...
	auto stacktraceEntry = *std::stacktrace::current(stacktraceSkip).begin();

	fs::path filepath = fs::relative(stacktraceEntry.source_file(), fs::current_path().parent_path());

	LoggerSingleton::_logger->log(
		spdlog::source_loc(
			filepath.string().c_str(),
			stacktraceEntry.source_line(),
			utils::parseStacktraceFunction(stacktraceEntry).c_str()
		),
		(spdlog::level::level_enum)level,
		fmt,
		std::forward<Args>(args)...
	);
}

template<class... Args>
void UniversalLogger::operator()(Level level, std::format_string<Args...> fmt, Args&&... args) {
	logImpl(level, 2, fmt, std::forward<Args>(args)...);
}

template<Level L>
template<class... Args>
void LeveledLogger<L>::operator()(std::format_string<Args...> fmt, Args&&... args) {
	logImpl(L, 2, fmt, std::forward<Args>(args)...);
}

template<Level L>
consteval LeveledLogger<L>::operator Level() const noexcept {
	return L;
}

} // namespace arch::log::_details

