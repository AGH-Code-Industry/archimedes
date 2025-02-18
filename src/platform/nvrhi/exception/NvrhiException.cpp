#include "NvrhiException.h"

namespace arch::gfx::nvrhi::exception {

NvrhiException::NvrhiException(const std::string& message, const std::source_location& location):
	Exception("Nvrhi", message, location) {}

} // namespace arch::gfx::nvrhi::exception
