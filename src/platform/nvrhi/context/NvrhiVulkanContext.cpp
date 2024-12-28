#define NOMINMAX
#include "NvrhiVulkanContext.h"

#include <vulkan/vulkan.hpp>

#include "../../vulkan/VulkanUtils.h"
#include "../NvrhiMessageCallback.h"
#include "../NvrhiUtils.h"
#include "Window.h"
#include "nvrhi/validation.h"
#include "nvrhi/vulkan.h"

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

namespace arch::gfx::nvrhi {

NvrhiVulkanContext::NvrhiVulkanContext(bool enableValidationLayers):
	VulkanContext(true),
	_enableValidationLayers(enableValidationLayers) {}

NvrhiVulkanContext::~NvrhiVulkanContext() {
	shutdown();
}

void NvrhiVulkanContext::init(const Ref<Window>& window) {
	_createSurface(window);
	initDevice(_surface);

	::nvrhi::vulkan::DeviceDesc desc;
	desc.instance = getInstance();
	desc.physicalDevice = getPhysicalDevice();
	desc.device = VulkanContext::getDevice();
	desc.allocationCallbacks = getAllocator();

	Queue gQueue = getQueue(QueueType::graphics);
	desc.graphicsQueue = gQueue.queue;
	desc.graphicsQueueIndex = gQueue.index;

	Queue tQueue = getQueue(QueueType::transfer);
	desc.transferQueue = tQueue.queue;
	desc.transferQueueIndex = tQueue.index;

	Queue cQueue = getQueue(QueueType::compute);
	desc.computeQueue = cQueue.queue;
	desc.computeQueueIndex = cQueue.index;

	desc.errorCB = MessageCallback::GetInstance();

	desc.instanceExtensions = const_cast<const char**>(DEVICE_EXTENSIONS.data());
	desc.numInstanceExtensions = DEVICE_EXTENSIONS.size();

	// Dynamically load the Vulkan-Hpp function pointers (used by NVRHI)
	{
		const vk::DynamicLoader dl;
		const PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr =
			dl.getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr");
		VULKAN_HPP_DEFAULT_DISPATCHER.init(desc.instance, vkGetInstanceProcAddr, desc.device);
	}

	_device = createDevice(desc);

	i32 width, height;
	glfwGetFramebufferSize(window->get(), &width, &height);

	onResize(width, height);
	_createFrameSemaphores();
}

void NvrhiVulkanContext::shutdown() {
	NvrhiContext::shutdown();

	if (_swapchain) {
		_destroySwapchain();
	}

	for (u32 i = 0; i < _acquireSemaphores.size(); i++) {
		vkDestroySemaphore(VulkanContext::getDevice(), _acquireSemaphores[i], getAllocator());
		vkDestroySemaphore(VulkanContext::getDevice(), _presentSemaphores[i], getAllocator());
	}
	_acquireSemaphores.clear();
	_presentSemaphores.clear();

	_device = nullptr;

	if (_surface) {
		vkDestroySurfaceKHR(getInstance(), _surface, getAllocator());
		_surface = nullptr;
	}
}

void NvrhiVulkanContext::onResize(u32 width, u32 height) {
	_preResizeFramebuffers();

	if (_swapchain) {
		_destroySwapchain();
	}

	_supportDetails = vulkan::VulkanSwapchain::SupportDetails::getSupportDetails(getPhysicalDevice(), _surface);
	_createSwapchain(width, height);

	_postResizeFramebuffers();
}

void NvrhiVulkanContext::beginFrame() {
	VkDevice device = VulkanContext::getDevice();

	const auto& semaphore = _acquireSemaphores[_currentAcquireSemaphore];

	VkResult res = {};

	constexpr int maxAttempts = 3;
	for (int attempt = 0; attempt < maxAttempts; ++attempt) {
		res = vkAcquireNextImageKHR(
			device,
			_swapchain,
			std::numeric_limits<u64>::max(), // timeout
			semaphore,
			vk::Fence(),
			&_currentFrame
		);

		if (res == VK_ERROR_OUT_OF_DATE_KHR) {
			_supportDetails = vulkan::VulkanSwapchain::SupportDetails::getSupportDetails(getPhysicalDevice(), _surface);

			onResize(
				_supportDetails.capabilities.currentExtent.width,
				_supportDetails.capabilities.currentExtent.height
			);
		} else {
			break;
		}
	}

	_currentAcquireSemaphore = (_currentAcquireSemaphore + 1) % _acquireSemaphores.size();

	if (res == VK_SUCCESS) {
		// Schedule the wait. The actual wait operation will be submitted when the app executes any command list.
		_device->queueWaitForSemaphore(::nvrhi::CommandQueue::Graphics, semaphore, 0);
	}
}

void NvrhiVulkanContext::present() {
	const auto& semaphore = _presentSemaphores[_currentPresentSemaphore];

	_device->queueSignalSemaphore(::nvrhi::CommandQueue::Graphics, semaphore, 0);

	// NVRHI buffers the semaphores and signals them when something is submitted to a queue.
	// Call 'executeCommandLists' with no command lists to actually signal the semaphore.
	_device->executeCommandLists(nullptr, 0);

	VkPresentInfoKHR info = {};
	info.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
	info.waitSemaphoreCount = 1;
	info.pWaitSemaphores = &semaphore;

	info.swapchainCount = 1;
	info.pSwapchains = &_swapchain;
	info.pImageIndices = &_currentFrame;

	const Queue& presentQueue = getQueue(QueueType::presentaion);

	VkResult res = vkQueuePresentKHR(presentQueue.queue, &info);
	if (!(res == VK_SUCCESS || res == VK_ERROR_OUT_OF_DATE_KHR)) {
		throw vulkan::exceptions::VulkanException("Failed to present swapchain");
	}

	_currentPresentSemaphore = (_currentPresentSemaphore + 1) % _presentSemaphores.size();

#ifndef _WIN32
	if (_vsync) {
		vkQueueWaitIdle(presentQueue.queue);
	}
#endif

	while (_framesInFlight.size() >= _maxFramesInFlight) {
		auto query = _framesInFlight.front();
		_framesInFlight.pop();

		_device->waitEventQuery(query);

		_queryPool.push_back(query);
	}

	::nvrhi::EventQueryHandle query;
	if (!_queryPool.empty()) {
		query = _queryPool.back();
		_queryPool.pop_back();
	} else {
		query = _device->createEventQuery();
	}

	_device->resetEventQuery(query);
	_device->setEventQuery(query, ::nvrhi::CommandQueue::Graphics);
	_framesInFlight.push(query);
}

int NvrhiVulkanContext::getCurrentFrameIndex() {
	return _currentFrame;
}

uint2 NvrhiVulkanContext::getFramebufferSize() const {
	return { _extent.width, _extent.height };
}

::nvrhi::DeviceHandle NvrhiVulkanContext::getDevice() {
	return _device;
}

u32 NvrhiVulkanContext::_getBackBufferCount() const {
	return _frames.size();
}

::nvrhi::TextureHandle NvrhiVulkanContext::_getBackBuffer(u32 index) const {
	return _frames[index].handle;
}

void NvrhiVulkanContext::_createSurface(const Ref<Window>& window) {
	vulkan::VulkanUtils::vkAssert(
		glfwCreateWindowSurface(getInstance(), window->get(), getAllocator(), &_surface),
		"Failed to create window surface."
	);
}

void NvrhiVulkanContext::_createSwapchain(i32 width, i32 height) {
	_surfaceFormat = _supportDetails.formats[0];
	for (const auto& availableFormat : _supportDetails.formats) {
		if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&
			availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) {
			_surfaceFormat = availableFormat;
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

	if (_supportDetails.capabilities.currentExtent.width != std::numeric_limits<u32>::max()) {
		_extent = _supportDetails.capabilities.currentExtent;
	} else {
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
		.imageFormat = _surfaceFormat.format,
		.imageColorSpace = _surfaceFormat.colorSpace,
		.imageExtent = _extent,
		.imageArrayLayers = 1,
		.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT,
		.preTransform = _supportDetails.capabilities.currentTransform,
		.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
		.presentMode = presentMode,
		.clipped = VK_TRUE,
	};

	std::array queueFamilyIndices = {
		getQueue(QueueType::graphics).index,
		getQueue(QueueType::presentaion).index,
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

	VkDevice device = VulkanContext::getDevice();
	VkAllocationCallbacks* allocator = getAllocator();

	vulkan::VulkanUtils::vkAssert(
		vkCreateSwapchainKHR(device, &createInfo, allocator, &_swapchain),
		"Failed to create swap chain!"
	);

	vkGetSwapchainImagesKHR(device, _swapchain, &imageCount, nullptr);
	std::vector<VkImage> swapchainImages(imageCount);
	vkGetSwapchainImagesKHR(device, _swapchain, &imageCount, swapchainImages.data());
	_frames.resize(imageCount);
	for (u64 i = 0; i < _frames.size(); ++i) {
		_frames[i].image = swapchainImages[i];

		GraphicsFormat fromat = vulkan::VulkanUtils::getFormat(_surfaceFormat.format);

		::nvrhi::TextureDesc textureDesc;
		textureDesc.width = width;
		textureDesc.height = height;
		textureDesc.format = NvrhiUtils::getFormat(fromat);
		textureDesc.debugName = "Swap chain image";
		textureDesc.initialState = ::nvrhi::ResourceStates::Present;
		textureDesc.keepInitialState = true;
		textureDesc.isRenderTarget = true;

		_frames[i].handle = getDevice()->createHandleForNativeTexture(
			::nvrhi::ObjectTypes::VK_Image,
			::nvrhi::Object(swapchainImages[i]),
			textureDesc
		);
	}
}

void NvrhiVulkanContext::_destroySwapchain() {
	VkDevice device = VulkanContext::getDevice();

	vkDeviceWaitIdle(device);

	vkDestroySwapchainKHR(device, _swapchain, getAllocator());

	_swapchain = nullptr;

	_frames.clear();
}

void NvrhiVulkanContext::_createFrameSemaphores() {
	VkDevice device = VulkanContext::getDevice();

	_acquireSemaphores.reserve(_maxFramesInFlight + 1);
	_presentSemaphores.reserve(_maxFramesInFlight + 1);
	for (u32 i = 0; i < _maxFramesInFlight + 1; i++) {
		VkSemaphoreCreateInfo semaphoreInfo{};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

		VkSemaphore acquireSemaphore;
		vkCreateSemaphore(device, &semaphoreInfo, getAllocator(), &acquireSemaphore);

		VkSemaphore presentSemaphore;
		vkCreateSemaphore(device, &semaphoreInfo, getAllocator(), &presentSemaphore);

		_acquireSemaphores.push_back(acquireSemaphore);
		_presentSemaphores.push_back(presentSemaphore);
	}
}

void NvrhiVulkanContext::_destroyFrameSemaphores() {
	for (u32 i = 0; i < _acquireSemaphores.size(); i++) {
		vkDestroySemaphore(VulkanContext::getDevice(), _acquireSemaphores[i], getAllocator());
		vkDestroySemaphore(VulkanContext::getDevice(), _presentSemaphores[i], getAllocator());
	}
	_acquireSemaphores.clear();
	_presentSemaphores.clear();
}

} // namespace arch::gfx::nvrhi
