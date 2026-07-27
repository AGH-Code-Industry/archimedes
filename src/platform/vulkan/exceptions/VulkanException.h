#pragma once

#include <archimedes/Exception.h>

namespace arch::gfx::vulkan::exceptions {

class VulkanException final: public Exception {
public:
	VulkanException(const std::string& message, const std::stacktrace& stacktrace = std::stacktrace::current());
};

} // namespace arch::gfx::vulkan::exceptions
