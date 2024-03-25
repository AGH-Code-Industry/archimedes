#if ARCHIMEDES_WINDOWS
#include <net/_utilitiesWindows.h>
#elif ARCHIMEDES_UNIX
#include <net/_utilitiesUnix.h>
#endif

#include <chrono>

namespace arch::net {
/// @brief Timeout type, milliseconds.
///
using TimeoutMs = std::chrono::milliseconds::rep;
}