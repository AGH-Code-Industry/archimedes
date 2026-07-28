#include "VulkanException.h"

namespace arch::gfx::vulkan::exceptions {

VulkanException::VulkanException(const std::string& message, const std::stacktrace& stacktrace):
	Exception("Vulkan", message, stacktrace) {}

} // namespace arch::gfx::vulkan::exceptions
