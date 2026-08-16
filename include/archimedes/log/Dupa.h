#pragma once

#include <archimedes/Math.h>
#include <archimedes/log/Logger.h>
#include <archimedes/utils/SimpleSourceLocation.h>

namespace arch {

/// @brief Official support for D.U.P.A.
/// @brief D.U.P.A - Debugging Under Polish Approach
/// @param level - log level (log::critical by default)
/// @return Sequence number of this call
extern u32 dupa(
	log::Level level = log::critical,
	const utils::SimpleSourceLocation loc = utils::SimpleSourceLocation::current()
) noexcept;

/// @brief Official support for D.U.P.A.
/// @brief D.U.P.A - Debugging Under Polish Approach
/// @param newVal - new value for sequence number
/// @param level - log level (log::critical by default)
/// @return Sequence number of this call
extern u32 dupa(
	const u32 newVal,
	log::Level = log::critical,
	const utils::SimpleSourceLocation loc = utils::SimpleSourceLocation::current()
) noexcept;

} // namespace arch
