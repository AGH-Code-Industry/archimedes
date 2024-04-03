#include "platform/vulkan/VulkanUtils.h"

#include <set>

#include "Logger.h"
#include <GLFW/glfw3.h>

namespace arch::gfx::vulkan {

bool VulkanUtils::areValidationLayersEnabled() {
	return true;
}

std::vector<const char*> VulkanUtils::getValidationLayers() {
	if (!areValidationLayersEnabled()) {
		return {};
	}

	std::vector<const char*> layers{VALIDATION_LAYERS.begin(), VALIDATION_LAYERS.end()};

	// Get all avalible layers
	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	// Filter out missing layers
	std::erase_if(layers, [&](auto&& layer) {
		for (auto&& l : availableLayers) {
			if (std::strcmp(layer, l.layerName) == 0) {
				return false;
			}
		}
		return true;
	});

	Logger::trace("Available layers {}:", layers.size());
	for (auto&& layer : availableLayers) {
		Logger::trace("  - {}", layer.layerName);
	}

	return layers;
}

std::vector<const char*> VulkanUtils::getRequiredExtensions() {
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	std::vector extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	if (areValidationLayersEnabled()) {
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	return extensions;
}

SwapchainSupportDetails VulkanUtils::getSwapchainSupportDetails(VkPhysicalDevice device, VkSurfaceKHR surface) {
	SwapchainSupportDetails swapchain;

	vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &swapchain.capabilities);

	u32 formatCount;
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, nullptr);

	swapchain.formats.resize(formatCount);
	vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount, swapchain.formats.data());

	u32 presentModeCount;
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, nullptr);

	swapchain.presentModes.resize(presentModeCount);
	vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, swapchain.presentModes.data());
	return swapchain;
}

int VulkanUtils::getDeviceScore(
	VkPhysicalDevice device,
	VkSurfaceKHR surface,
	QueueFamilyIndices& indices,
	SwapchainSupportDetails& swapchain
) {
	int score = 0;
	VkPhysicalDeviceProperties deviceProperties;
	VkPhysicalDeviceFeatures deviceFeatures;
	vkGetPhysicalDeviceProperties(device, &deviceProperties);
	vkGetPhysicalDeviceFeatures(device, &deviceFeatures);

	// Device features requirements
	{
		if (!deviceFeatures.geometryShader) {
			return -1;
		}

		if (deviceFeatures.samplerAnisotropy) {
			score += 1;
		}
	}

	// Device Extension requirements
	{
		uint32_t extensionCount;
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr);

		std::vector<VkExtensionProperties> availableExtensions(extensionCount);
		vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data());

		std::set<std::string> requiredExtensions = {DEVICE_EXTENSIONS.begin(), DEVICE_EXTENSIONS.end()};

		for (const auto& [extensionName, specVersion] : availableExtensions) {
			requiredExtensions.erase(extensionName);
		}

		if (!requiredExtensions.empty()) {
			return -1;
		}
	}

	// Device Queue Families requirements
	{
		uint32_t queueFamilyCount = 0;
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

		std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
		vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

		u32 i = 0;
		for (const auto& queueFamily : queueFamilies) {
			VkBool32 presentSupport = false;
			vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

			if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT && presentSupport) {
				if (indices.graphicsFamily != indices.presentFamily || indices.graphicsFamily == ~0u) {
					indices.graphicsFamily = i;
					indices.presentFamily = i;
				}
			} else if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) {
				indices.graphicsFamily = i;
			} else if (presentSupport) {
				indices.presentFamily = i;
			}

			if (queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT && indices.transferFamily == ~0u) {
				indices.transferFamily = i;
			}

			if (indices.isComplete()) {
				break;
			}

			i++;
		}

		if (!indices.isComplete()) {
			return -1;
		}
	}

	// Required Swapchain Support
	swapchain = getSwapchainSupportDetails(device, surface);

	if (swapchain.formats.empty()) {
		return -1;
	}

	if (swapchain.presentModes.empty()) {
		return -1;
	}

	switch (deviceProperties.deviceType) {
		case VK_PHYSICAL_DEVICE_TYPE_OTHER:
		case VK_PHYSICAL_DEVICE_TYPE_MAX_ENUM: score += 1; break;

		case VK_PHYSICAL_DEVICE_TYPE_CPU:
		case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU:
		case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU: score += 10; break;

		case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU: score += 20; break;
	}

	return score;
}

u32 VulkanUtils::findMemoryType(VkPhysicalDevice device, uint32_t typeFilter, VkMemoryPropertyFlags properties) {
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(device, &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
			return i;
		}
	}

	throw std::runtime_error("failed to find suitable memory type!");
}

VkFormat VulkanUtils::findSupportedFormat(
	VkPhysicalDevice device,
	const std::vector<VkFormat>& candidates,
	VkImageTiling tiling,
	VkFormatFeatureFlags features
) {
	for (VkFormat format : candidates) {
		VkFormatProperties props;
		vkGetPhysicalDeviceFormatProperties(device, format, &props);

		if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
			return format;
		}

		if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
			return format;
		}
	}
	throw std::runtime_error("failed to find supported format!");
}

u32 VulkanUtils::debugCallback(
	VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
	VkDebugUtilsMessageTypeFlagsEXT messageType,
	const VkDebugUtilsMessengerCallbackDataEXT* pCallbackData,
	void* pUserData
) {
	LogLevel level = LogLevel::debug;

	if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
		level = LogLevel::error;
	} else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT) {
		level = LogLevel::warn;
	} else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT) {
		level = LogLevel::info;
	} else if (messageSeverity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT) {
		level = LogLevel::trace;
	}

	Logger::log(level, "[Vulkan] {}", pCallbackData->pMessage);
	return VK_FALSE;
}

} // namespace arch::gfx::vulkan
