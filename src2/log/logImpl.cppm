module;

#include <archimedes/BuildInfo.h>
#include <archimedes/Defines.h>
#include <archimedes/ForceInline.h>
//
#include <filesystem>
#include <format>
#include <memory>
#include <print>
#include <stacktrace>

#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/spdlog.h>

export module arch.log:logImpl;

import arch.utils.parseStacktraceFunction;
import :Level;
import :LoggerSingleton;

export namespace std {

using std::partial_ordering;
using std::strong_ordering;
using std::weak_ordering;

} // namespace std

export namespace arch::log::_details {

template<class... Args>
ARCHIMEDES_FORCE_INLINE void logImpl(
	Level level,
	const unsigned int stacktraceSkip,
	std::format_string<Args...> fmt,
	Args&&... args
) {
	namespace fs = std::filesystem;

	// 0: this
	// 1: logger
	// 2: actual location
	// ...
	std::println("{}", std::stacktrace::current());
	auto stacktraceEntry = *std::stacktrace::current(stacktraceSkip).begin();

	fs::path filepath = fs::relative(stacktraceEntry.source_file(), fs::current_path().parent_path());

	if constexpr (buildinfo::Type::current == buildinfo::Type::Debug) {
		if (!LoggerSingleton::_logger) {
			std::println("Logger not initialized!");
			return;
		}
	}

	LoggerSingleton::_logger->log(
		spdlog::source_loc(
			filepath.string().c_str(),
			stacktraceEntry.source_line(),
			stacktraceEntry.description().c_str()
			// utils::parseStacktraceFunction(stacktraceEntry).c_str()
		),
		(spdlog::level::level_enum)level,
		fmt,
		std::forward<Args>(args)...
	);
}

} // namespace arch::log::_details
