#pragma once

#include "VulkanTypes.h"
#include "Window.h"
#include "buffer/VulkanBufferManager.h"
#include "gfx/Renderer.h"
#include <volk.h>

namespace arch::gfx::vulkan {

class VulkanRenderer final: public Renderer {
public:
	VulkanRenderer(): Renderer(createRef<buffer::VulkanBufferManager>()) {}

	void init(const Ref<Window>& window) override;
	void shutdown() override;

	void render(const Ref<Mesh>& mesh, const Mat4x4& transform) override;

private:

	void _createInstance();
	void _setupDebugMessage();
	void _createSurface();

	void _pickPhisicalDevice();
	void _createLogicalDevice();

	void _createSwapchain();
	void _recreateSwapchain();
	void _cleanupSwapchain();

	void _createImageViews();

public:
	VkAllocationCallbacks* allocator = nullptr;
	VkInstance instance = nullptr;
	VkDebugUtilsMessengerEXT debugMessenger = nullptr;

	VkPhysicalDevice physicalDevice = nullptr;
	VkDevice device = nullptr;

	VkSurfaceKHR surface = nullptr;

	VkQueue transferQueue = nullptr;
	VkQueue graphicsQueue = nullptr;
	VkQueue presentQueue = nullptr;

	VkSwapchainKHR swapchain = nullptr;
	VkFormat swapchainImageFormat = VK_FORMAT_UNDEFINED;
	VkExtent2D swapchainExtent = {0, 0};

	QueueFamilyIndices indices;
	SwapchainSupportDetails swapchainSupportDetails;

	struct FrameData {
		VkImage image;
		VkImageView imageView;
		VkFramebuffer framebuffer;
	};

	std::vector<FrameData> swapchainFrames;
};

} // namespace arch::gfx::vulkan
