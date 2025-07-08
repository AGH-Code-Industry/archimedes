#pragma once

#include <Mmath.h>

namespace arch::input {

/// @brief Enum with all supported key codes
/// @details Includes keyboard and mouse keys
enum class MouseButtonCode : u32 {
	begin = 350,
	left = begin,
	right,
	middle,

	first = left,
	second,
	third,
	fourth,
	fifth,
	sixth,
	seventh,
	eighth,
};

} // namespace arch::input
