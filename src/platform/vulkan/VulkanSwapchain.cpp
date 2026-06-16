#include "VulkanSwapchain.h"

#include <algorithm>

#include "VulkanContext.h"
#include "VulkanUtils.h"
#include <archimedes/Window.h>

namespace arch::gfx::vulkan {

VulkanSwapchain::VulkanSwapchain(const Ref<VulkanContext>& context, const Ref<Window>& window):
	_context(context),
	_window(window) {
	_createSurface();
}

VulkanSwapchain::~VulkanSwapchain() {
	_cleanupSwapchain();

	vkDestroySurfaceKHR(_context->getInstance(), _surface, _context->getAllocator());
	_surface = nullptr;
}

void VulkanSwapchain::updateSwapchain() {
	if (_swapchain == nullptr) {
		_supportDetails = SupportDetails::getSupportDetails(_context->getPhysicalDevice(), _surface);

		_createSwapchain();
	} else {
		_recreateSwapchain();
	}
}

void VulkanSwapchain::_createSurface() {
	VulkanUtils::vkAssert(
		glfwCreateWindowSurface(_context->getInstance(), _window->get(), _context->getAllocator(), &_surface),
		"Failed to create window surface."
	);
}

void VulkanSwapchain::_createSwapchain() {
	VkSurfaceFormatKHR surfaceFormat = _supportDetails.formats[0];
	for (const auto& availableFormat : _supportDetails.formats) {
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
			availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			surfaceFormat = availableFormat;
			break;
		}
	}

	VkPresentModeKHR presentMode = VK_PRESENT_MODE_FIFO_KHR;
#if false // Triple buffering - currently not supported
	for (const auto& availablePresentMode : swapchainSupportDetails.presentModes) {
		if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
			presentMode = availablePresentMode;
			break;
		}
	}
#endif

	_format = surfaceFormat.format;

	if (_supportDetails.capabilities.currentExtent.width != std::numeric_limits<u32>::max()) {
		_extent = _supportDetails.capabilities.currentExtent;
	} else {
		i32 width, height;
		glfwGetFramebufferSize(_window->get(), &width, &height);

		_extent = { std::clamp(
						(u32)width,
						_supportDetails.capabilities.minImageExtent.width,
						_supportDetails.capabilities.maxImageExtent.width
					),
					std::clamp(
						(u32)height,
						_supportDetails.capabilities.minImageExtent.height,
						_supportDetails.capabilities.maxImageExtent.height
					) };
	}

	u32 imageCount = _supportDetails.capabilities.minImageCount + 1;
	if (_supportDetails.capabilities.maxImageCount > 0 && imageCount > _supportDetails.capabilities.maxImageCount) {
		imageCount = _supportDetails.capabilities.maxImageCount;
	}

	VkSwapchainCreateInfoKHR createInfo = {
		.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
		.surface = _surface,
		.minImageCount = imageCount,
		.imageFormat = surfaceFormat.format,
		.imageColorSpace = surfaceFormat.colorSpace,
		.imageExtent = _extent,
		.imageArrayLayers = 1,
		.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
		.preTransform = _supportDetails.capabilities.currentTransform,
		.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
		.presentMode = presentMode,
		.clipped = VK_TRUE,
	};

	std::array queueFamilyIndices = {
		_context->getQueue(VulkanContext::QueueType::graphics).index,
		_context->getQueue(VulkanContext::QueueType::presentaion).index,
	};

	if (queueFamilyIndices[0] != queueFamilyIndices[1]) {
		createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
		createInfo.queueFamilyIndexCount = (u32)queueFamilyIndices.size();
		createInfo.pQueueFamilyIndices = queueFamilyIndices.data();
	} else {
		createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
		createInfo.queueFamilyIndexCount = 0; // Optional
		createInfo.pQueueFamilyIndices = nullptr; // Optional
	}

	VkDevice device = _context->getDevice();
	VkAllocationCallbacks* allocator = _context->getAllocator();

	VulkanUtils::vkAssert(
		vkCreateSwapchainKHR(device, &createInfo, allocator, &_swapchain),
		"Failed to create swap chain!"
	);

	vkGetSwapchainImagesKHR(device, _swapchain, &imageCount, nullptr);
	std::vector<VkImage> swapchainImages(imageCount);
	vkGetSwapchainImagesKHR(device, _swapchain, &imageCount, swapchainImages.data());
	_frames.resize(imageCount);
	for (u64 i = 0; i < _frames.size(); ++i) {
		_frames[i].image = swapchainImages[i];
	}

	// Image Views
	for (u64 i = 0; i < _frames.size(); i++) {
		VkImageViewCreateInfo imageViewCreateInfo = {
			.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
			.image = _frames[i].image,
			.viewType = VK_IMAGE_VIEW_TYPE_2D,
			.format = _format,
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
			}
		};

		VulkanUtils::vkAssert(
			vkCreateImageView(device, &imageViewCreateInfo, allocator, &_frames[i].imageView),
			"Failed to create image view!"
		);
	}
}

void VulkanSwapchain::_cleanupSwapchain() {
	VkDevice device = _context->getDevice();
	VkAllocationCallbacks* allocator = _context->getAllocator();

	for (const auto& frame : _frames) {
		vkDestroyImageView(device, frame.imageView, allocator);
	}
	_frames.clear();

	vkDestroySwapchainKHR(device, _swapchain, allocator);
	_swapchain = nullptr;
}

void VulkanSwapchain::_recreateSwapchain() {
	vkDeviceWaitIdle(_context->getDevice());

	_cleanupSwapchain();

	_supportDetails = SupportDetails::getSupportDetails(_context->getPhysicalDevice(), _surface);

	_createSwapchain();
}

VulkanSwapchain::SupportDetails VulkanSwapchain::SupportDetails::getSupportDetails(
	VkPhysicalDevice device,
	VkSurfaceKHR surface
) {
	SupportDetails swapchain;

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

} // namespace arch::gfx::vulkan
