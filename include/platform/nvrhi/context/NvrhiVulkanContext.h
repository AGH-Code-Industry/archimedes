#pragma once

#include "platform/nvrhi/NvrhiContext.h"
#include "platform/vulkan/VulkanContext.h"

namespace arch::gfx::nvrhi {

class NvrhiVulkanContext final:
	public NvrhiContext,
	public vulkan::VulkanContext,
	public std::enable_shared_from_this<NvrhiVulkanContext> {
public:
	explicit NvrhiVulkanContext();
	~NvrhiVulkanContext() override = default;

	void init(const Ref<Window>& window) override;

public:
	::nvrhi::DeviceHandle getDevice() override;

private:
	::nvrhi::DeviceHandle _device;

	Ref<vulkan::VulkanSwapchain> m_swapchain;
};

} // namespace arch::gfx::nvrhi
