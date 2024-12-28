#include "VulkanContext.h"

#include "VulkanUtils.h"
#include "exceptions/VulkanException.h"
#include <GLFW/glfw3.h>
#include <volk.h>

namespace arch::gfx::vulkan {

VulkanContext::VulkanContext(bool enableValidationLayers): _enableValidationLayers(enableValidationLayers) {
	VulkanUtils::vkAssert(volkInitialize(), "Failed to initialize volk.");

	_createInstance();

	volkLoadInstance(_instance);

	if (_enableValidationLayers) {
		_setupDebugMessage();
	}
}

VulkanContext::~VulkanContext() {
	if (_device) {
		vkDestroyDevice(_device, _allocator);
		_device = nullptr;
	}

	if (_debugMessenger) {
		vkDestroyDebugUtilsMessengerEXT(_instance, _debugMessenger, _allocator);
		_debugMessenger = nullptr;
	}

	if (_instance) {
		vkDestroyInstance(_instance, _allocator);
		_instance = nullptr;
	}
}

void VulkanContext::initDevice(VkSurfaceKHR surface) {
	_pickPhysicalDevice(surface);
	_createLogicalDevice();

	volkLoadDevice(_device);
}

namespace details {

static u32 debugCallback(
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

} // namespace details

const VulkanContext::Queue& VulkanContext::getQueue(QueueType type) const {
	switch (type) {
		case QueueType::graphics:	 return _queues.graphics;
		case QueueType::presentaion: return _queues.presentation;
		case QueueType::compute:	 return _queues.compute;
		case QueueType::transfer:	 return _queues.transfer;
		default:					 throw exceptions::VulkanException("Invalid queue type.");
	}
}

i32 VulkanContext::findMemoryType(u32 typeFilter, VkMemoryPropertyFlags properties) const {
	VkPhysicalDeviceMemoryProperties memProperties;
	vkGetPhysicalDeviceMemoryProperties(_physicalDevice, &memProperties);

	for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
		if ((typeFilter & (1 << i)) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
			return i;
		}
	}

	return -1;
}

void VulkanContext::_createInstance() {
	VkApplicationInfo appInfo{
		.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
		.pApplicationName = "Hello Triangle",
		.applicationVersion = VK_MAKE_VERSION(1, 0, 0),
		.pEngineName = "Archimedes",
		.engineVersion = VK_MAKE_VERSION(1, 0, 0),
		.apiVersion = VK_API_VERSION_1_3,
	};

	std::vector layers = _getValidationLayers();
	std::vector extensions = _getRequiredExtensions();

	VkInstanceCreateInfo createInfo{
		.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		.pApplicationInfo = &appInfo,
		.enabledLayerCount = (u32)layers.size(),
		.ppEnabledLayerNames = layers.data(),
		.enabledExtensionCount = (u32)extensions.size(),
		.ppEnabledExtensionNames = extensions.data(),
	};

	VulkanUtils::vkAssert(vkCreateInstance(&createInfo, _allocator, &_instance), "Failed to create Vulkan instance.");
}

void VulkanContext::_setupDebugMessage() {
	if (vkCreateDebugUtilsMessengerEXT == nullptr) {
		Logger::warn("vkCreateDebugUtilsMessengerEXT is not available, debug messages will not be displayed.");
		return;
	}

	VkDebugUtilsMessengerCreateInfoEXT createInfo{
		.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
		.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
		.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
		.pfnUserCallback = details::debugCallback,
		.pUserData = nullptr,
	};

	Logger::info("[Vulkan] Setting up debug messenger");
	VulkanUtils::vkAssert(
		vkCreateDebugUtilsMessengerEXT(_instance, &createInfo, _allocator, &_debugMessenger),
		"Failed to setup debug messenger."
	);
}

void VulkanContext::_pickPhysicalDevice(VkSurfaceKHR surface) {
	u32 deviceCount = 0;
	vkEnumeratePhysicalDevices(_instance, &deviceCount, nullptr);

	if (deviceCount == 0) {
		throw exceptions::VulkanException("Failed to find GPUs with Vulkan support!");
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(_instance, &deviceCount, devices.data());

	i32 bestScore = -1;
	for (const auto& device : devices) {
		if (i32 score = _getDeviceScore(device, surface); score >= 0 && bestScore < score) {
			_physicalDevice = device;
			bestScore = score;
		}
	}

	if (_physicalDevice == VK_NULL_HANDLE) {
		throw exceptions::VulkanException("Failed to find a suitable GPU!");
	}

	_queues = _getDeviceQueues(_physicalDevice, surface);
}

void VulkanContext::_createLogicalDevice() {
	// Graphics Queue
	std::set queueFamilies = { _queues.graphics.index,
							   _queues.presentation.index,
							   _queues.transfer.index,
							   _queues.compute.index };

	f32 queuePriority = 1.f;
	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

	for (auto&& queueFamily : queueFamilies) {
		queueCreateInfos.push_back(
			{ .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
			  .queueFamilyIndex = queueFamily,
			  .queueCount = 1,
			  .pQueuePriorities = &queuePriority }
		);
	}

	VkPhysicalDeviceVulkan12Features vulkan12features = {
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES,
		.descriptorIndexing = true,
		.shaderSampledImageArrayNonUniformIndexing = true,
		.descriptorBindingPartiallyBound = true,
		.descriptorBindingVariableDescriptorCount = true,
		.runtimeDescriptorArray = true,
		.timelineSemaphore = true,
		.bufferDeviceAddress = true,
	};
	VkPhysicalDeviceVulkan13Features vulkan13features = {
		.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES,
		.pNext = &vulkan12features,
		.synchronization2 = true,
	};

	VkPhysicalDeviceFeatures deviceFeatures = {
		.imageCubeArray = true,
		.geometryShader = true,
		.tessellationShader = true,
		.dualSrcBlend = true,
		.fillModeNonSolid = true,
		.samplerAnisotropy = true,
		.textureCompressionBC = true,
		.fragmentStoresAndAtomics = true,
		.shaderImageGatherExtended = true,
		.shaderInt16 = true,
	};

	VkDeviceCreateInfo createInfo = {
		.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
		.pNext = &vulkan13features,
		.queueCreateInfoCount = (u32)queueCreateInfos.size(),
		.pQueueCreateInfos = queueCreateInfos.data(),

		.enabledExtensionCount = (u32)DEVICE_EXTENSIONS.size(),
		.ppEnabledExtensionNames = DEVICE_EXTENSIONS.data(),

		.pEnabledFeatures = &deviceFeatures,
	};

	// Backward compatibility
	{
		if (_areValidationLayersEnabled()) {
			createInfo.enabledLayerCount = (u32)VALIDATION_LAYERS.size();
			createInfo.ppEnabledLayerNames = VALIDATION_LAYERS.data();
		} else {
			createInfo.enabledLayerCount = 0;
		}
	}

	VulkanUtils::vkAssert(
		vkCreateDevice(_physicalDevice, &createInfo, _allocator, &_device),
		"Failed to create logical device!"
	);

	vkGetDeviceQueue(_device, _queues.graphics.index, 0, &_queues.graphics.queue);
	vkGetDeviceQueue(_device, _queues.presentation.index, 0, &_queues.presentation.queue);
	vkGetDeviceQueue(_device, _queues.transfer.index, 0, &_queues.transfer.queue);
	vkGetDeviceQueue(_device, _queues.compute.index, 0, &_queues.compute.queue);
}

bool VulkanContext::_areValidationLayersEnabled() const {
	return _enableValidationLayers;
}

std::vector<const char*> VulkanContext::_getValidationLayers() {
	if (!_areValidationLayersEnabled()) {
		return {};
	}

	std::vector<const char*> layers{ VALIDATION_LAYERS.begin(), VALIDATION_LAYERS.end() };

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

	Logger::trace("Available layers {}:", availableLayers.size());
	for (auto&& layer : availableLayers) {
		Logger::trace("  - {}", layer.layerName);
	}

	return layers;
}

std::vector<const char*> VulkanContext::_getRequiredExtensions() const {
	uint32_t glfwExtensionCount = 0;
	const char** glfwExtensions = glfwGetRequiredInstanceExtensions(&glfwExtensionCount);
	std::vector extensions(glfwExtensions, glfwExtensions + glfwExtensionCount);

	if (_areValidationLayersEnabled()) {
		extensions.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
	}

	return extensions;
}

int VulkanContext::_getDeviceScore(VkPhysicalDevice device, VkSurfaceKHR surface) {
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

		std::set<std::string> requiredExtensions = { DEVICE_EXTENSIONS.begin(), DEVICE_EXTENSIONS.end() };

		for (const auto& [extensionName, specVersion] : availableExtensions) {
			requiredExtensions.erase(extensionName);
		}

		if (!requiredExtensions.empty()) {
			return -1;
		}
	}

	// Device Queue Families requirements
	Queues queues = _getDeviceQueues(device, surface);
	if (!queues.isComplete()) {
		return -1;
	}

	// Required Swapchain Support
	VulkanSwapchain::SupportDetails swapchainSupport =
		VulkanSwapchain::SupportDetails::getSupportDetails(device, surface);

	if (swapchainSupport.formats.empty()) {
		return -1;
	}

	if (swapchainSupport.presentModes.empty()) {
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

VkFormat VulkanContext::findSupportedFormat(
	const std::vector<VkFormat>& candidates,
	VkImageTiling tiling,
	VkFormatFeatureFlags features
) const {
	for (VkFormat format : candidates) {
		VkFormatProperties props;
		vkGetPhysicalDeviceFormatProperties(_physicalDevice, format, &props);

		if (tiling == VK_IMAGE_TILING_LINEAR && (props.linearTilingFeatures & features) == features) {
			return format;
		}

		if (tiling == VK_IMAGE_TILING_OPTIMAL && (props.optimalTilingFeatures & features) == features) {
			return format;
		}
	}
	throw exceptions::VulkanException("Failed to find supported format!");
}

VulkanContext::Queues VulkanContext::_getDeviceQueues(VkPhysicalDevice device, VkSurfaceKHR surface) {
	Queues queues;

	uint32_t queueFamilyCount = 0;
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, nullptr);

	std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount);
	vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

	u32 i = 0;
	for (const auto& queueFamily : queueFamilies) {
		VkBool32 presentSupport = false;
		vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &presentSupport);

		if (queues.graphics.index == -1) {
			if (queueFamily.queueCount > 0 && (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)) {
				queues.graphics.index = i;
			}
		}

		if (queues.compute.index == -1) {
			if (queueFamily.queueCount > 0 && (queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT) &&
				!(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)) {
				queues.compute.index = i;
			}
		}

		if (queues.transfer.index == -1) {
			if (queueFamily.queueCount > 0 && (queueFamily.queueFlags & VK_QUEUE_TRANSFER_BIT) &&
				!(queueFamily.queueFlags & VK_QUEUE_COMPUTE_BIT) && !(queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT)) {
				queues.transfer.index = i;
			}
		}

		if (queues.presentation.index == -1) {
			if (queueFamily.queueCount > 0 && presentSupport) {
				queues.presentation.index = i;
			}
		}

		if (queues.isComplete()) {
			break;
		}

		i++;
	}

	return queues;
}

} // namespace arch::gfx::vulkan
