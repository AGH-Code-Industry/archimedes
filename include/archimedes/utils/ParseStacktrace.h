#pragma once

#include <stacktrace>

namespace arch::utils {

/// @brief Returns function name parsed from stacktrace_entry::description()
/// @details Function name format may differ between platforms
/// @param entry - entry to parse
std::string parseStacktraceFunction(const std::stacktrace_entry& entry);

} // namespace arch::utils
