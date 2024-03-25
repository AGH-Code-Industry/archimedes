#include "gfx/Renderer.h"

#include <Logger.h>

#include <array>
#include <vector>
#include <ranges>
#include <algorithm>

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

	std::vector layers = {
		"VK_LAYER_KHRONOS_validation",
	};

	uint32_t layerCount;
	vkEnumerateInstanceLayerProperties(&layerCount, nullptr);
	std::vector<VkLayerProperties> availableLayers(layerCount);
	vkEnumerateInstanceLayerProperties(&layerCount, availableLayers.data());

	layers.erase(std::ranges::remove_if(layers, [&](auto&& layer) {
		for (auto&& l : availableLayers)
			if (std::strcmp(layer, l.layerName) == 0) return false;
		return true;
	}).begin());

	Logger::info("Available layers {}:", layerCount);
	for (auto&& layer : availableLayers) {
		Logger::info("  - {}", layer.layerName);
	}

	VkInstanceCreateInfo create_info{
		.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		.pApplicationInfo = &appInfo,
		.enabledLayerCount = (uint32_t)layers.size(),
		.ppEnabledLayerNames = layers.data(),
		.enabledExtensionCount = (uint32_t)instanceExtensions.size(),
		.ppEnabledExtensionNames = instanceExtensions.data(),
	};

	VkInstance instance;
	VkResult status = vkCreateInstance(&create_info, nullptr, &instance);

	if (status != VK_SUCCESS)
		arch::Logger::error("Failed to create Vulkan instance.");

	Logger::info("Created Vulkan instance.");
}

}
