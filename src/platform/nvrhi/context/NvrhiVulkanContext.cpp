#include "platform/nvrhi/context/NvrhiVulkanContext.h"

#include <vulkan/vulkan.hpp>

#include "nvrhi/validation.h"
#include "nvrhi/vulkan.h"
#include "platform/vulkan/VulkanUtils.h"

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE

namespace arch::gfx::nvrhi {

struct MessageCallback: public ::nvrhi::IMessageCallback {
	void message(::nvrhi::MessageSeverity severity, const char* message) override {
		LogLevel level = LogLevel::debug;

		switch (severity) {
			case ::nvrhi::MessageSeverity::Fatal:	level = LogLevel::critical; break;
			case ::nvrhi::MessageSeverity::Error:	level = LogLevel::error; break;
			case ::nvrhi::MessageSeverity::Warning: level = LogLevel::warn; break;
			case ::nvrhi::MessageSeverity::Info:	level = LogLevel::info; break;
		}

		Logger::log(level, "[Nvrhi]: {}", message);
	}

	static MessageCallback* GetInstance() {
		static MessageCallback instance;
		return &instance;
	}
};

NvrhiVulkanContext::NvrhiVulkanContext(): VulkanContext(false) {}

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

	desc.errorCB = MessageCallback::GetInstance();

	desc.instanceExtensions = nullptr;
	desc.numInstanceExtensions = 0;

	// Dynamically load the Vulkan-Hpp function pointers (used by NVRHI)
	{
		const vk::DynamicLoader dl;
		const PFN_vkGetInstanceProcAddr vkGetInstanceProcAddr =
			dl.getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr");
		VULKAN_HPP_DEFAULT_DISPATCHER.init(desc.instance, vkGetInstanceProcAddr, desc.device);
	}

	_device = createDevice(desc);
}

::nvrhi::DeviceHandle NvrhiVulkanContext::getDevice() {
	return _device;
}

} // namespace arch::gfx::nvrhi
