#include "gfx/Renderer.h"

#include <Logger.h>

#include <array>
#include <vector>

namespace arch::gfx {

void Renderer::init() {
	volkInitialize();

	VkApplicationInfo appInfo {
		.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
		.pApplicationName = "Hello Triangle",
		.applicationVersion = VK_MAKE_VERSION(1, 0, 0),
		.pEngineName = "No Engine",
		.engineVersion = VK_MAKE_VERSION(1, 0, 0),
		.apiVersion = VK_API_VERSION_1_3,
	};

	std::array instanceExtensions = {
		VK_KHR_SURFACE_EXTENSION_NAME,
	};

	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	arch::Logger::info("Available layers: {}", layerCount);

	std::array layers = {
		"VK_LAYER_KHRONOS_validation",
	};

	VkInstanceCreateInfo create_info {
		.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		.pApplicationInfo = &appInfo,
		.enabledLayerCount = layers.size(),
		.ppEnabledLayerNames = layers.data(),
		.enabledExtensionCount = instanceExtensions.size(),
		.ppEnabledExtensionNames = instanceExtensions.data(),
	};

	VkInstance instance;
	VkResult status = vkCreateInstance(&create_info, nullptr, &instance);

	if (status != VK_SUCCESS)
		arch::Logger::error("Failed to create Vulkan instance.");

	Logger::info("Created Vulkan instance.");
}

}
