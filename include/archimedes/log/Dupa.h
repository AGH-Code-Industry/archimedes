#pragma once

#include <archimedes/Mmath.h>
#include <archimedes/log/Logger.h>

namespace arch {

/// @brief Official support for D.U.P.A.
/// @brief D.U.P.A - Debugging Under Polish Approach
/// @param level - log level (log::critical by default)
/// @return Sequence number of this call
extern u32 dupa(log::Level level = log::critical) noexcept;

/// @brief Official support for D.U.P.A.
/// @brief D.U.P.A - Debugging Under Polish Approach
/// @param newVal - new value for sequence number
/// @param level - log level (log::critical by default)
/// @return Sequence number of this call
extern u32 dupa(const u32 newVal, log::Level = log::critical) noexcept;

} // namespace arch
