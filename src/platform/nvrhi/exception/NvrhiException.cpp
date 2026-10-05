#include "NvrhiException.h"

namespace arch::gfx::nvrhi::exception {

NvrhiException::NvrhiException(const std::string& message, const utils::SimpleSourceLocation& loc):
	Exception("Nvrhi", message, loc) {}

} // namespace arch::gfx::nvrhi::exception
