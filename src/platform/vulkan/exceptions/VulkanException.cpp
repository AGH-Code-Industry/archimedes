#include "platform/vulkan/exceptions/VulkanException.h"

namespace arch::gfx::vulkan::exceptions {

VulkanException::VulkanException(const std::string& message, const std::source_location& location):
	Exception("Vulkan", message, location) {}

} // namespace arch::gfx::vulkan::exceptions
