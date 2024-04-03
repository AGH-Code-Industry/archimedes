#pragma once

#include <vector>

#include "mmath.h"
#include <volk.h>

namespace arch::gfx::vulkan {

struct QueueFamilyIndices {
	u32 transferFamily = ~0u;
	u32 graphicsFamily = ~0u;
	u32 presentFamily = ~0u;

	bool isComplete() const { return transferFamily != ~0u && graphicsFamily != ~0u && presentFamily != ~0u; }
};

struct SwapchainSupportDetails {
	VkSurfaceCapabilitiesKHR capabilities{};
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR> presentModes;
};

} // namespace arch::gfx::vulkan
