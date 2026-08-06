module;

#include <atomic>

#include <archimedes/BuildInfo.h>
#include <archimedes/ForceInline.h>

export module arch.dupa;

import arch.log;

static std::atomic<unsigned int> dupaVal = 0;

export namespace arch {

/// @brief Official support for D.U.P.A.
/// @brief D.U.P.A - Debugging Under Polish Approach
/// @param level - log level (log::critical by default)
/// @return Sequence number of this call
ARCHIMEDES_FORCE_INLINE unsigned int dupa(log::Level level = log::Level::critical) noexcept {
	auto val = dupaVal.fetch_add(1, std::memory_order::relaxed);
	arch::log::_details::logImpl(level, buildinfo::noInlining ? 2 : 0, "dupa {}", val);
	return val;
}

/// @brief Official support for D.U.P.A.
/// @brief D.U.P.A - Debugging Under Polish Approach
/// @param newVal - new value for sequence number
/// @param level - log level (log::critical by default)
/// @return Sequence number of this call
ARCHIMEDES_FORCE_INLINE unsigned int dupa(const unsigned int newVal, log::Level level = log::Level::critical) noexcept {
	dupaVal.store(newVal + 1, std::memory_order::relaxed);
	arch::log::_details::logImpl(level, buildinfo::noInlining ? 2 : 0, "dupa {}", newVal);
	return newVal;
}

} // namespace arch
