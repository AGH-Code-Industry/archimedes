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
	VulkanRenderer(): Renderer(RenderingAPI::vulkan) {}

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
	void onResize(u32 width, u32 height) override;

	void beginFrame() override;
	void present() override;

private:
	Ref<buffer::VulkanBufferManager> _bufferManager;
	Ref<texture::VulkanTextureManager> _textureManager;

	Ref<VulkanContext> _context;
	Ref<VulkanSwapchain> _swapchain;

	struct FrameData {
		VkFramebuffer framebuffer;

		VkCommandBuffer commandBuffer;
		VkSemaphore imageAvailableSemaphore;
		VkSemaphore renderFinishedSemaphore;
		VkFence inFlightFence;
	};

	std::vector<FrameData> _frames;
	std::uint32_t _frameIndex = 0;
	Ref<texture::VulkanTexture> _depthTexture;
};

} // namespace arch::gfx::vulkan
