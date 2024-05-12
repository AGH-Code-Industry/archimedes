#pragma once

#include "VulkanContext.h"
#include "VulkanSwapchain.h"
#include "Window.h"
#include "gfx/Renderer.h"

namespace arch::gfx::vulkan {

class VulkanRenderer final: public Renderer {
public:
	VulkanRenderer() = default;
	~VulkanRenderer() override = default;

	void init(const Ref<Window>& window) override;
	void shutdown() override;

	void render(const Ref<Mesh>& mesh, const Mat4x4& transform) override;

private:
	void _createRenderPass();
	void _craeteDepthTexture();
	void _createFramebuffers();

public:
	VkAllocationCallbacks* allocator = nullptr;
	VkInstance instance = nullptr;
	VkDebugUtilsMessengerEXT debugMessenger = nullptr;

	VkPhysicalDevice physicalDevice = nullptr;
	VkDevice device = nullptr;

	Ref<VulkanContext> context;
	Ref<VulkanSwapchain> swapchain;

	VkRenderPass renderPass;

	struct FrameData {
		VkFramebuffer framebuffers;

		VkCommandBuffer commandBuffer;
		VkSemaphore imageAvailableSemaphore;
		VkSemaphore renderFinishedSemaphore;
		VkFence inFlightFence;
	};

	std::vector<FrameData> frames;
	std::uint32_t _frameIndex = 0;
	Ref<texture::VulkanTexture2D> depthTexture;
};

} // namespace arch::gfx::vulkan
