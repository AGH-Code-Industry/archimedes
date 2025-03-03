#pragma once

#include "Exception.h"

namespace arch::gfx::nvrhi::exception {

class NvrhiException final: public Exception {
public:
	NvrhiException(const std::string& message, const std::source_location& location = std::source_location::current());
};

} // namespace arch::gfx::nvrhi::exceptions
