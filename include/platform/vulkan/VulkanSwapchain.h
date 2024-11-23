#pragma once
#include <vector>

#include "Mmath.h"
#include "Ref.h"
#include <volk.h>

namespace arch {
class Window;
}

namespace arch::gfx::vulkan {

class VulkanContext;

class VulkanSwapchain {
public:
	struct SupportDetails {
		VkSurfaceCapabilitiesKHR capabilities{};
		std::vector<VkSurfaceFormatKHR> formats;
		std::vector<VkPresentModeKHR> presentModes;
	};

	struct Frame {
		VkImage image;
		VkImageView imageView;
	};

public:
	VulkanSwapchain(const Ref<VulkanContext>& context, const Ref<Window>& window);
	~VulkanSwapchain();

	VulkanSwapchain(const VulkanSwapchain&) = delete;
	VulkanSwapchain& operator=(const VulkanSwapchain&) = delete;

	VulkanSwapchain(VulkanSwapchain&&) = default;
	VulkanSwapchain& operator=(VulkanSwapchain&&) = default;

public:
	void updateSwapchain();

public:
	VkSurfaceKHR getSurface() const { return _surface; }

	VkSwapchainKHR getSwapchain() const { return _swapchain; }

	VkFormat getFormat() const { return _format; }

	VkExtent2D getExtent() const { return _extent; }

	u64 getFrameCount() const { return _frames.size(); }

	const Frame& getFrame(int index) const { return _frames[index]; }

	SupportDetails getSupportDetails(VkPhysicalDevice device) const;

private:
	void _createSurface();

	void _createSwapchain();
	void _cleanupSwapchain();
	void _recreateSwapchain();

private:
	Ref<VulkanContext> _context;
	Ref<Window> _window;

	VkSurfaceKHR _surface = nullptr;
	VkSwapchainKHR _swapchain = nullptr;

	VkPresentModeKHR _presentMode = {};
	VkFormat _format = VK_FORMAT_UNDEFINED;
	VkExtent2D _extent = { 0, 0 };

	std::vector<Frame> _frames;

	SupportDetails _supportDetails;
};

} // namespace arch::gfx::vulkan
