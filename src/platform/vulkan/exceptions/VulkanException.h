#pragma once

#include <archimedes/Exception.h>

namespace arch::gfx::vulkan::exceptions {

class VulkanException final: public Exception {
public:
	VulkanException(
		const std::string& message,
		const utils::SimpleSourceLocation& loc = utils::SimpleSourceLocation::current()
	);
};

} // namespace arch::gfx::vulkan::exceptions
