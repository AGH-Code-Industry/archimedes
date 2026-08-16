#pragma once

#include <filesystem>
#include <print>

#include "Logger.h"
#include <archimedes/BuildInfo.h>
#include <archimedes/utils/ParseStacktrace.h>

namespace arch::log::_details {

template<class... Args>
void logImpl(Level level, const utils::SimpleSourceLocation loc, std::format_string<Args...> fmt, Args&&... args) {
	namespace fs = std::filesystem;

	fs::path filepath = fs::relative(loc.fileName(), fs::current_path().parent_path());

	if constexpr (buildinfo::Type::current == buildinfo::Type::Debug) {
		if (!LoggerSingleton::_logger) {
			std::println("Logger not initialized!");
			return;
		}
	}

	LoggerSingleton::_logger->log(
		spdlog::source_loc(
			filepath.string().c_str(),
			loc.line(),
			"" // function name empty, spdlog doesn't log it
		),
		(spdlog::level::level_enum)level,
		fmt,
		std::forward<Args>(args)...
	);
}

template<class... Args>
void UniversalLogger::operator()(Level level, utils::SourceAwareFormatString<Args...> fmt, Args&&... args) {
	logImpl(level, fmt.loc, fmt.fmt, std::forward<Args>(args)...);
}

template<Level L>
template<class... Args>
void LeveledLogger<L>::operator()(utils::SourceAwareFormatString<Args...> fmt, Args&&... args) {
	logImpl(L, fmt.loc, fmt.fmt, std::forward<Args>(args)...);
}

template<Level L>
consteval LeveledLogger<L>::operator Level() const noexcept {
	return L;
}

} // namespace arch::log::_details

