#pragma once

#include <archimedes/Exception.h>

namespace arch::gfx::nvrhi::exception {

class NvrhiException final: public Exception {
public:
	NvrhiException(const std::string& message, const std::stacktrace& stacktrace = std::stacktrace::current());
};

} // namespace arch::gfx::nvrhi::exception
