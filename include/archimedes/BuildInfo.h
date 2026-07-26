#pragma once

namespace arch::buildinfo {

/// @brief Enum representing build types
enum class Type {
	/// @brief Debug build
	Debug,
	/// @brief Release build
	Release,
	/// @brief RelWithDebInfo build
	RelWithDebInfo,
	/// @brief Current build type, equal to CMAKE_BUILD_TYPE
	current = Release
};

/// @brief Enum representing compiler
enum class Compiler {
	/// @brief GCC compiler
	gcc,
	/// @brief MSVC compiler
	msvc,
	/// @brief Current compiler
	current = msvc
};

} // namespace arch
