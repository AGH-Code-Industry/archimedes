#pragma once

#include <archimedes/Exception.h>

namespace arch::utils {

class BadResAccess: public arch::Exception {
public:
	BadResAccess(std::string_view msg, const std::stacktrace& stacktrace = std::stacktrace::current()) noexcept;
};

} // namespace arch::utils
