module;

#include <archimedes/Defines.h>
//
#include <spdlog/spdlog.h>

export module arch.log:Level;

export namespace arch::log {

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
	critical = spdlog::level::critical,
};

} // namespace arch::log
