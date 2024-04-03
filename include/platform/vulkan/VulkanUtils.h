#pragma once
#include <array>
#include <vector>

#include "VulkanTypes.h"

namespace arch::gfx::vulkan {

class VulkanUtils {
public:

	static constexpr std::array DEVICE_EXTENSIONS = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME,
		VK_KHR_MAINTENANCE_1_EXTENSION_NAME,
	};

	static constexpr std::array VALIDATION_LAYERS = {"VK_LAYER_KHRONOS_validation"};

	static bool areValidationLayersEnabled();

	static std::vector<const char*> getValidationLayers();

	static std::vector<const char*> getRequiredExtensions();

	static SwapchainSupportDetails getSwapchainSupportDetails(VkPhysicalDevice device, VkSurfaceKHR surface);

	static int getDeviceScore(
		VkPhysicalDevice device,
		VkSurfaceKHR surface,
		QueueFamilyIndices& indices,
		SwapchainSupportDetails& swapchain
	);

	static u32 findMemoryType(VkPhysicalDevice device, u32 typeFilter, VkMemoryPropertyFlags properties);

	static VkFormat findSupportedFormat(
		VkPhysicalDevice device,
		const std::vector<VkFormat>& candidates,
		VkImageTiling tiling,
		VkFormatFeatureFlags features
	);

	static u32 debugCallback(
		VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
		VkDebugUtilsMessageTypeFlagsEXT messageType,
		const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
		void* pUserData
	);
};

} // namespace arch::gfx::vulkan