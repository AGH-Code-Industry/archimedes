#include "NvrhiException.h"

namespace arch::gfx::nvrhi::exception {

NvrhiException::NvrhiException(const std::string& message, const std::stacktrace& stacktrace):
	Exception("Nvrhi", message, stacktrace) {}

} // namespace arch::gfx::nvrhi::exception
