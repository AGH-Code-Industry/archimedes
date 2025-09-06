#pragma once

#if ARCHIMEDES_WINDOWS
#include <archimedes/net/_UtilitiesWindows.h>
#elif ARCHIMEDES_UNIX
#include <archimedes/net/_UtilitiesUnix.h>
#endif

#include <chrono>

namespace arch::net {

/// @brief Timeout type, milliseconds.
///
using TimeoutMs = std::chrono::milliseconds::rep;

} // namespace arch::net

