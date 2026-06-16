#pragma once

#include <array>
#include <set>

#include "VulkanSwapchain.h"
#include "exceptions/VulkanException.h"
#include <archimedes/Mmath.h>
#include <volk.h>

namespace arch::gfx::vulkan {

class VulkanContext {
public:
	enum class QueueType : std::size_t {
		none,

		graphics,
		presentaion,
		transfer,
		compute,
	};

	struct Queue {
		u32 index = ~0u;
		QueueType type = QueueType::none;
		VkQueue queue = nullptr;

		Queue(QueueType type): type(type) {}
	};

	struct Queues {
		Queue graphics{ QueueType::graphics };
		Queue presentation{ QueueType::presentaion };
		Queue transfer{ QueueType::transfer };
		Queue compute{ QueueType::compute };

		bool isComplete() const {
			return graphics.index != ~0u && presentation.index != ~0u && transfer.index != ~0u && compute.index != ~0u;
		}
	};

public:
	VulkanContext(bool enableValidationLayers = true);

	virtual ~VulkanContext();

	VulkanContext(const VulkanContext&) = delete;
	VulkanContext& operator=(const VulkanContext&) = delete;

	VulkanContext(VulkanContext&&) = default;
	VulkanContext& operator=(VulkanContext&&) = default;

public:
	void initDevice(VkSurfaceKHR surface);

public:
	VkInstance getInstance() const { return _instance; }

	VkAllocationCallbacks* getAllocator() const { return _allocator; }

	VkPhysicalDevice getPhysicalDevice() const { return _physicalDevice; }

	VkDevice getDevice() const { return _device; }

	const Queues& getQueues() const { return _queues; }

	const Queue& getQueue(QueueType type) const;

	i32 findMemoryType(u32 typeFilter, VkMemoryPropertyFlags properties) const;
	VkFormat findSupportedFormat(
		const std::vector<VkFormat>& candidates,
		VkImageTiling tiling,
		VkFormatFeatureFlags features
	) const;

private:

	void _createInstance();
	void _setupDebugMessage();

	void _pickPhysicalDevice(VkSurfaceKHR surface);
	void _createLogicalDevice();

protected:
	static constexpr std::array DEVICE_EXTENSIONS = {
		VK_KHR_SWAPCHAIN_EXTENSION_NAME,
		VK_KHR_MAINTENANCE_1_EXTENSION_NAME,
		// VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME,
		VK_KHR_TIMELINE_SEMAPHORE_EXTENSION_NAME,

		// VK_KHR_DYNAMIC_RENDERING_EXTENSION_NAME,
		VK_KHR_SYNCHRONIZATION_2_EXTENSION_NAME,

		// VK_EXT_DESCRIPTOR_INDEXING_EXTENSION_NAME,
		// VK_EXT_BUFFER_DEVICE_ADDRESS_EXTENSION_NAME,
	};

	static constexpr std::array VALIDATION_LAYERS = { "VK_LAYER_KHRONOS_validation" };

	bool _areValidationLayersEnabled() const;

	std::vector<const char*> _getValidationLayers();

	std::vector<const char*> _getRequiredExtensions() const;

	static int _getDeviceScore(VkPhysicalDevice device, VkSurfaceKHR surface);
	static Queues _getDeviceQueues(VkPhysicalDevice device, VkSurfaceKHR surface);

private:
	bool _enableValidationLayers = true;

	VkInstance _instance = nullptr;
	VkDebugUtilsMessengerEXT _debugMessenger = nullptr;

	VkAllocationCallbacks* _allocator = nullptr;

	VkPhysicalDevice _physicalDevice = nullptr;
	VkDevice _device = nullptr;

	std::set<const char*> _enabledLayers;
	std::set<const char*> _instanceExtensions;

	Queues _queues;
};

} // namespace arch::gfx::vulkan
