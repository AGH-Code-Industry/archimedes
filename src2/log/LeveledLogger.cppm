module;

#include <compare>
//

#include <archimedes/BuildInfo.h>
#include <archimedes/Defines.h>
#include <archimedes/ForceInline.h>
//
#include <filesystem>
#include <format>
#include <print>
#include <source_location>
#include <stacktrace>

#include <spdlog/spdlog.h>

export module arch.log:LeveledLogger;

import :Level;
import :logImpl;

template<class Char, class... Args>
struct BasicFormatString {
	const std::basic_format_string<Char, Args...> fmt;
	const std::source_location loc;

	template<class T>
	requires std::convertible_to<const T&, std::basic_string_view<Char>>
	consteval BasicFormatString(const T& str, const std::source_location location = std::source_location::current()):
		fmt(str),
		loc{ location } {}
};

template<class... Args>
using FormatString = BasicFormatString<char, std::type_identity_t<Args>...>;

export namespace arch::log {

namespace _details {

template<Level L>
struct LeveledLogger {
	/// @brief Logging operator
	/// @param level - log level
	/// @param fmt - format string
	/// @param args... - arguments to format
	template<class... Args>
	static void operator()(FormatString<Args...> fmt, Args&&... args) {
		::arch::log::_details::logImpl(L, buildinfo::noInlining ? 2 : 0, fmt.fmt, std::forward<Args>(args)...);
	}

	consteval operator Level() const noexcept { return L; }
};

} // namespace _details

/// @brief Logger for detailed trace information
export _details::LeveledLogger<Level::trace> trace{};
/// @brief Logger for debugging information
export _details::LeveledLogger<Level::debug> debug{};
/// @brief Logger for general information
export _details::LeveledLogger<Level::info> info{};
/// @brief Logger for information representing a minor problem that has occurred
export _details::LeveledLogger<Level::warn> warn{};
/// @brief Logger for information representing a serious problem that has occurred
export _details::LeveledLogger<Level::error> error{};
/// @brief Logger for information representing a critical problem that has occurred
export _details::LeveledLogger<Level::critical> critical{};

} // namespace arch::log
