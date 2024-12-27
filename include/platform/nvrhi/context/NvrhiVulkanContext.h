#pragma once

#include <queue>

#include "platform/nvrhi/NvrhiContext.h"
#include "platform/vulkan/VulkanContext.h"
#include <nvrhi/vulkan.h>

namespace arch::gfx::nvrhi {

class NvrhiVulkanContext final: public vulkan::VulkanContext, public NvrhiContext {
public:
	explicit NvrhiVulkanContext(bool enableValidationLayers = true);
	~NvrhiVulkanContext() override;

	void init(const Ref<Window>& window) override;
	void shutdown() override;

	void onResize(u32 width, u32 height) override;

	void beginFrame() override;
	void present() override;

public:
	int getCurrentFrameIndex() override;
	uint2 getFramebufferSize() const override;

	::nvrhi::DeviceHandle getDevice() override;

public:
	struct Frame {
		VkImage image;
		::nvrhi::TextureHandle handle;
	};

protected:
	u32 _getBackBufferCount() const override;
	::nvrhi::TextureHandle _getBackBuffer(uint32_t index) const override;

private:
	void _createSurface(const Ref<Window>& window);
	void _createSwapchain(i32 width, i32 height);
	void _destroySwapchain();
	void _createFrameSemaphores();
	void _destroyFrameSemaphores();

public:

private:
	bool _enableValidationLayers;

	::nvrhi::vulkan::DeviceHandle _device;

	VkSurfaceKHR _surface = nullptr;
	VkSwapchainKHR _swapchain = nullptr;

	VkPresentModeKHR _presentMode = {};
	VkSurfaceFormatKHR _surfaceFormat = {};
	VkExtent2D _extent = { 0, 0 };

	uint32_t _currentFrame = -1;
	std::vector<Frame> _frames;

	uint32_t _currentAcquireSemaphore = 0;
	uint32_t _currentPresentSemaphore = 0;
	std::vector<VkSemaphore> _acquireSemaphores;
	std::vector<VkSemaphore> _presentSemaphores;

	vulkan::VulkanSwapchain::SupportDetails _supportDetails;

	std::queue<::nvrhi::EventQueryHandle> _framesInFlight;
	std::vector<::nvrhi::EventQueryHandle> _queryPool;

	const int _maxFramesInFlight = 2;
};

} // namespace arch::gfx::nvrhi
