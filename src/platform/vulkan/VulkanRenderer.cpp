#include "platform/vulkan/VulkanRenderer.h"

#include <vector>

#include "Logger.h"
#include "Window.h"
#include "platform/vulkan/VulkanUtils.h"
#include <GLFW/glfw3.h>

namespace arch::gfx::vulkan {

void VulkanRenderer::init(const Ref<Window>& window) {
	_window = window;

	volkInitialize();

	_createInstance();

	volkLoadInstance(instance);

	_setupDebugMessage();

	_createSurface();

	_pickPhisicalDevice();

	_createLogicalDevice();

	volkLoadDevice(device);

	Logger::info("Created Vulkan instance.");
}

void VulkanRenderer::shutdown() {
	vkDestroyDevice(device, allocator);
	device = nullptr;

	vkDestroySurfaceKHR(instance, surface, allocator);
	surface = nullptr;

	vkDestroyDebugUtilsMessengerEXT(instance, debugMessenger, allocator);
	debugMessenger = nullptr;

	vkDestroyInstance(instance, allocator);
	instance = nullptr;
}

void VulkanRenderer::render(const Ref<Mesh>& mesh, const Mat4x4& transform) {}

void VulkanRenderer::_createInstance() {
	VkApplicationInfo appInfo{
		.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
		.pApplicationName = "Hello Triangle",
		.applicationVersion = VK_MAKE_VERSION(1, 0, 0),
		.pEngineName = "Archimedes",
		.engineVersion = VK_MAKE_VERSION(1, 0, 0),
		.apiVersion = VK_API_VERSION_1_3,
	};

	std::vector layers = VulkanUtils::getValidationLayers();
	std::vector extensions = VulkanUtils::getRequiredExtensions();

	VkInstanceCreateInfo createInfo{
		.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
		.pApplicationInfo = &appInfo,
		.enabledLayerCount = (uint32_t)layers.size(),
		.ppEnabledLayerNames = layers.data(),
		.enabledExtensionCount = (uint32_t)extensions.size(),
		.ppEnabledExtensionNames = extensions.data(),
	};

	VkResult status = vkCreateInstance(&createInfo, allocator, &instance);

	if (status != VK_SUCCESS) {
		Logger::error("Failed to create Vulkan instance.");
	}
}

void VulkanRenderer::_setupDebugMessage() {
	VkDebugUtilsMessengerCreateInfoEXT createInfo{
		.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
		.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
		.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
			VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
		.pfnUserCallback = VulkanUtils::debugCallback,
		.pUserData = nullptr,
	};

	vkCreateDebugUtilsMessengerEXT(instance, &createInfo, allocator, &debugMessenger);
}

void VulkanRenderer::_createSurface() {
	VkResult status = glfwCreateWindowSurface(instance, _window->get(), allocator, &surface);
	if (status != VK_SUCCESS) {
		throw std::runtime_error("failed to create window surface!");
	}
}

void VulkanRenderer::_pickPhisicalDevice() {
	uint32_t deviceCount = 0;
	vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

	if (deviceCount == 0) {
		throw std::runtime_error("failed to find GPUs with Vulkan support!");
	}

	std::vector<VkPhysicalDevice> devices(deviceCount);
	vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

	int bestScore = -1;
	for (const auto& device : devices) {
		QueueFamilyIndices currentIndices;
		SwapchainSupportDetails currentSwapchainSupportDetails;
		if (int score = VulkanUtils::getDeviceScore(device, surface, currentIndices, currentSwapchainSupportDetails);
			score >= 0 && bestScore < score) {
			physicalDevice = device;
			indices = currentIndices;
			swapchainSupportDetails = currentSwapchainSupportDetails;
			bestScore = score;
		}
	}

	if (physicalDevice == VK_NULL_HANDLE) {
		throw std::runtime_error("failed to find a suitable GPU!");
	}
}

void VulkanRenderer::_createLogicalDevice() {
	float queuePriority = 1.f;

	std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;

	// Graphics Queue
	queueCreateInfos.push_back({ .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
								 .queueFamilyIndex = indices.graphicsFamily,
								 .queueCount = 1,
								 .pQueuePriorities = &queuePriority });

	if (indices.presentFamily != indices.graphicsFamily) {
		// Present Queue
		queueCreateInfos.push_back({ .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
									 .queueFamilyIndex = indices.presentFamily,
									 .queueCount = 1,
									 .pQueuePriorities = &queuePriority });
	}

	if (indices.transferFamily != indices.graphicsFamily && indices.transferFamily != indices.presentFamily) {
		// Present Queue
		queueCreateInfos.push_back({ .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
									 .queueFamilyIndex = indices.transferFamily,
									 .queueCount = 1,
									 .pQueuePriorities = &queuePriority });
	}

	VkPhysicalDeviceFeatures deviceFeatures = {};

	VkDeviceCreateInfo createInfo = {
		.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
		.queueCreateInfoCount = static_cast<u32>(queueCreateInfos.size()),
		.pQueueCreateInfos = queueCreateInfos.data(),

		.enabledExtensionCount = static_cast<u32>(VulkanUtils::DEVICE_EXTENSIONS.size()),
		.ppEnabledExtensionNames = VulkanUtils::DEVICE_EXTENSIONS.data(),

		.pEnabledFeatures = &deviceFeatures,
	};

	// Backward compatibility
	{
		if (VulkanUtils::areValidationLayersEnabled()) {
			createInfo.enabledLayerCount = static_cast<u32>(VulkanUtils::VALIDATION_LAYERS.size());
			createInfo.ppEnabledLayerNames = VulkanUtils::VALIDATION_LAYERS.data();
		} else {
			createInfo.enabledLayerCount = 0;
		}
	}

	if (vkCreateDevice(physicalDevice, &createInfo, allocator, &device) != VK_SUCCESS) {
		throw std::runtime_error("failed to create logical device!");
	}

	vkGetDeviceQueue(device, indices.transferFamily, 0, &transferQueue);
	vkGetDeviceQueue(device, indices.graphicsFamily, 0, &graphicsQueue);
	vkGetDeviceQueue(device, indices.presentFamily, 0, &presentQueue);
}

void VulkanRenderer::_createSwapchain() {
	VkSurfaceFormatKHR surfaceFormat = swapchainSupportDetails.formats[0];
	for (const auto& availableFormat : swapchainSupportDetails.formats) {
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
			availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			surfaceFormat = availableFormat;
			break;
		}
	}

	VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;
	for (const auto& availablePresentMode : swapchainSupportDetails.presentModes) {
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
			presentMode = availablePresentMode;
			break;
		}
	}

	VkExtent2D extent;
	if (swapchainSupportDetails.capabilities.currentExtent.width != std::numeric_limits<u32>::max()) {
		extent = swapchainSupportDetails.capabilities.currentExtent;
	} else {
		int width, height;
		glfwGetFramebufferSize(_window->get(), &width, &height);

		extent = { std::clamp(
					   static_cast<u32>(width),
					   swapchainSupportDetails.capabilities.minImageExtent.width,
					   swapchainSupportDetails.capabilities.maxImageExtent.width
				   ),
				   std::clamp(
					   static_cast<u32>(height),
					   swapchainSupportDetails.capabilities.minImageExtent.height,
					   swapchainSupportDetails.capabilities.maxImageExtent.height
				   ) };
	}

	u32 imageCount = swapchainSupportDetails.capabilities.minImageCount + 1;
	if (swapchainSupportDetails.capabilities.maxImageCount > 0 &&
		imageCount > swapchainSupportDetails.capabilities.maxImageCount) {
		imageCount = swapchainSupportDetails.capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR createInfo = {
		.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
		.surface = surface,
		.minImageCount = imageCount,
		.imageFormat = surfaceFormat.format,
		.imageColorSpace = surfaceFormat.colorSpace,
		.imageExtent = extent,
		.imageArrayLayers = 1,
		.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
		.preTransform = swapchainSupportDetails.capabilities.currentTransform,
		.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
		.presentMode = presentMode,
		.clipped = VK_TRUE,
	};

	std::array queueFamilyIndices = { indices.graphicsFamily, indices.presentFamily };

	if (indices.graphicsFamily != indices.presentFamily) {
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = (u32)queueFamilyIndices.size();
		createInfo.pQueueFamilyIndices = queueFamilyIndices.data();
	} else {
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0; // Optional
		createInfo.pQueueFamilyIndices = nullptr; // Optional
	}

	if (vkCreateSwapchainKHR(device, &createInfo, allocator, &swapchain) != VK_SUCCESS) {
		throw std::runtime_error("failed to create swap chain!");
	}

	vkGetSwapchainImagesKHR(device, swapchain, &imageCount, nullptr);
	std::vector<VkImage> swapchainImages(imageCount);
	vkGetSwapchainImagesKHR(device, swapchain, &imageCount, swapchainImages.data());
	swapchainFrames.resize(imageCount);
	for (int i = 0; i < swapchainFrames.size(); ++i) {
		swapchainFrames[i].image = swapchainImages[i];
	}

	swapchainExtent = extent;
	swapchainImageFormat = surfaceFormat.format;
}

void VulkanRenderer::_createImageViews() {
	for (u64 i = 0; i < swapchainFrames.size(); i++) {
		VkImageViewCreateInfo createInfo = {
			.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
			.image = swapchainFrames[i].image,
			.viewType = VK_IMAGE_VIEW_TYPE_2D,
			.format = swapchainImageFormat,
			.components = {
				.r = VK_COMPONENT_SWIZZLE_IDENTITY,
				.g = VK_COMPONENT_SWIZZLE_IDENTITY,
				.b = VK_COMPONENT_SWIZZLE_IDENTITY,
				.a = VK_COMPONENT_SWIZZLE_IDENTITY,
			},
			.subresourceRange = {
				.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
				.baseMipLevel = 0,
				.levelCount = 1,
				.baseArrayLayer = 0,
				.layerCount = 1,
			},
		};

		if (vkCreateImageView(device, &createInfo, allocator, &swapchainFrames[i].imageView) != VK_SUCCESS) {
			throw std::runtime_error("failed to create image views!");
		}
	}
}

} // namespace arch::gfx::vulkan
