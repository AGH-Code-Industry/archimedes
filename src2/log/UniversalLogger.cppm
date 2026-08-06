module;

#include <archimedes/BuildInfo.h>
#include <archimedes/Defines.h>
#include <archimedes/ForceInline.h>
//
#include <filesystem>
#include <format>
#include <print>
#include <stacktrace>

#include <spdlog/spdlog.h>

export module arch.log:UniversalLogger;

import :Level;
import :logImpl;

export namespace arch::log {

export namespace _details {

struct UniversalLogger {
	/// @brief Logging operator
	/// @param level - log level
	/// @param fmt - format string
	/// @param args... - arguments to format
	template<class... Args>
	ARCHIMEDES_FORCE_INLINE static void operator()(Level level, std::format_string<Args...> fmt, Args&&... args) {
		logImpl(level, buildinfo::noInlining ? 2 : 0, fmt, std::forward<Args>(args)...);
	}
};

} // namespace _details

export _details::UniversalLogger logger{};

} // namespace arch::log
