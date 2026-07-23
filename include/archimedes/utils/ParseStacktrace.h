#pragma once

#include <stacktrace>

namespace arch::utils {

std::string parseStacktraceEntry(const std::stacktrace_entry& entry);

}
