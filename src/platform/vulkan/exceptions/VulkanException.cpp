#include "VulkanException.h"

namespace arch::gfx::vulkan::exceptions {

VulkanException::VulkanException(const std::string& message, const utils::SimpleSourceLocation& loc):
	Exception("Vulkan", message, loc) {}

} // namespace arch::gfx::vulkan::exceptions
