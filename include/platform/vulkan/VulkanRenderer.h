#pragma once

#include "VulkanContext.h"
#include "VulkanSwapchain.h"
#include "Window.h"
#include "gfx/Renderer.h"
#include "texture/VulkanTexture.h"

namespace arch::gfx::vulkan {

namespace buffer {
class VulkanBufferManager;
}

namespace texture {
class VulkanTextureManager;
}

class VulkanRenderer final: public Renderer {
public:
	VulkanRenderer() = default;
	~VulkanRenderer() override = default;

	void init(const Ref<Window>& window) override;
	void shutdown() override;

	Ref<gfx::buffer::BufferManager> getBufferManager() override;

	Ref<TextureManager> getTextureManager() override;

	void render(const Ref<Mesh>& mesh, const Mat4x4& transform) override;

private:
	void _createDepthTexture();
	void _createFrames();

public:
	void prepareFrame() override;
	void beginFrame() override;
	void endFrame() override;
	void present() override;
	void setClearColor(Color color) override;
	void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
	void setScissor(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

public:
	Ref<buffer::VulkanBufferManager> _bufferManager;
	Ref<texture::VulkanTextureManager> _textureManager;

	Ref<VulkanContext> context;
	Ref<VulkanSwapchain> swapchain;

	struct FrameData {
		VkFramebuffer framebuffer;

		VkCommandBuffer commandBuffer;
		VkSemaphore imageAvailableSemaphore;
		VkSemaphore renderFinishedSemaphore;
		VkFence inFlightFence;
	};

	std::vector<FrameData> frames;
	std::uint32_t _frameIndex = 0;
	Ref<texture::VulkanTexture> depthTexture;
};

} // namespace arch::gfx::vulkan
