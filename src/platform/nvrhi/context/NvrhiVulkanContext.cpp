#include "platform/nvrhi/context/NvrhiVulkanContext.h"

#include "nvrhi/vulkan.h"
#include "platform/vulkan/VulkanUtils.h"

namespace arch::gfx::nvrhi {

NvrhiVulkanContext::NvrhiVulkanContext() {}

void NvrhiVulkanContext::init(const Ref<Window>& window) {
	m_swapchain = std::make_unique<vulkan::VulkanSwapchain>(
		std::static_pointer_cast<vulkan::VulkanContext>(shared_from_this()),
		window
	);
	initDevice(m_swapchain);
	m_swapchain->updateSwapchain();

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

	// desc.errorCB = messageCallback;

	desc.instanceExtensions = nullptr;
	desc.numInstanceExtensions = 0;

	_device = ::nvrhi::vulkan::createDevice(desc);
}

::nvrhi::DeviceHandle NvrhiVulkanContext::getDevice() {
	return _device;
}

} // namespace arch::gfx::nvrhi
