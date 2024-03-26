#if ARCHIMEDES_WINDOWS
#include <net/_UtilitiesWindows.h>
#elif ARCHIMEDES_UNIX
#include <net/_UtilitiesUnix.h>
#endif

#include <chrono>

namespace arch::net {

/// @brief Timeout type, milliseconds.
///
using TimeoutMs = std::chrono::milliseconds::rep;

} // namespace arch::net
