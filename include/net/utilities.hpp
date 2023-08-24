#ifdef WIN32
#include <net/__utilities_windows.hpp>
#elif defined unix
#include <net/__utilities_unix.hpp>
#endif

#include <chrono>

namespace arch::net {
/// @brief Timeout type, milliseconds.
///
using timeout_t = std::chrono::milliseconds::rep;
}