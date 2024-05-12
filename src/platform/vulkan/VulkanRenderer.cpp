#include "platform/vulkan/VulkanRenderer.h"

#include "Logger.h"
#include "Window.h"
#include "platform/vulkan/VulkanUtils.h"
#include "platform/vulkan/exceptions/VulkanException.h"
#include <GLFW/glfw3.h>

namespace arch::gfx::vulkan {

void VulkanRenderer::init(const Ref<Window>& window) {
	_window = window;

	VulkanUtils::vkAssert(volkInitialize(), "Failed to initialize volk.");

	_createInstance();

	context = createRef<VulkanContext>();

	swapchain = createRef<VulkanSwapchain>(context, window);

	context->initDevice(swapchain);

	swapchain->updateSwapchain();

	Logger::info("Created Vulkan instance.");
}

void VulkanRenderer::shutdown() {
	if (swapchain.use_count() > 1) {
		Logger::warn("Swapchain is still in use.");
	}
	swapchain.reset();

	if (context.use_count() > 1) {
		Logger::warn("Context is still in use.");
	}
	context.reset();
}

	vkDestroySurfaceKHR(instance, surface, allocator);
	surface = nullptr;

	vkDestroyDebugUtilsMessengerEXT(instance, debugMessenger, allocator);
	debugMessenger = nullptr;

	vkDestroyInstance(instance, allocator);
	instance = nullptr;
}

void VulkanRenderer::render(const Ref<Mesh>& mesh, const Mat4x4& transform) {}

void VulkanRenderer::_createRenderPass() {
	std::vector<VkAttachmentDescription> attachments{
		{
			.format = swapchain->getFormat(),
			.samples = VK_SAMPLE_COUNT_1_BIT,
			.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
			.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
			.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
			.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
			.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
			.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
		 },

		{
			.format = VulkanUtils::getFormat(depthTexture->getFormat()),
			.samples = VK_SAMPLE_COUNT_1_BIT,
			.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
			.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
			.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
			.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
			.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
			.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
		 }
	};

	VkAttachmentReference colorAttachmentRef = { .attachment = 0, .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };
	VkAttachmentReference depthAttachmentRef = { .attachment = 1,
												 .layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL };

	VkSubpassDescription subpass = {
		.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
		.colorAttachmentCount = 1,
		.pColorAttachments = &colorAttachmentRef,
		.pDepthStencilAttachment = &depthAttachmentRef,
	};

	std::vector<VkSubpassDependency> dependencies{
		{
			.srcSubpass = VK_SUBPASS_EXTERNAL,
			.dstSubpass = 0,
			.srcStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
			.dstStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
			.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT,
			.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT,
			.dependencyFlags = 0,
		 },

		{
			.srcSubpass = 0,
			.dstSubpass = VK_SUBPASS_EXTERNAL,
			.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
			.dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
			.srcAccessMask = 0,
			.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_COLOR_ATTACHMENT_READ_BIT,
			.dependencyFlags = 0,
		 }
	};

	VkRenderPassCreateInfo createInfo = {
		.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
		.attachmentCount = (u32)attachments.size(),
		.pAttachments = attachments.data(),
		.subpassCount = 1,
		.pSubpasses = &subpass,
		.dependencyCount = (u32)dependencies.size(),
		.pDependencies = dependencies.data(),

	};
	VulkanUtils::vkAssert(
		vkCreateRenderPass(context->getDevice(), &createInfo, context->getAllocator(), &renderPass),
		"Failed to create render pass."
	);
}

void VulkanRenderer::_craeteDepthTexture() {
	// Depthbuffer
	VkFormat format = context->findSupportedFormat(
		{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
		VK_IMAGE_TILING_OPTIMAL,
		VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
	);

	VkExtent2D extent = swapchain->getExtent();

	vkDestroySwapchainKHR(device, swapchain, allocator);
	swapchain = nullptr;
}

void VulkanRenderer::_createFramebuffers() {
	VkExtent2D extent = swapchain->getExtent();

	frames.resize(swapchain->getFrameCount());
	for (int i = 0; i < frames.size(); ++i) {
		const VulkanSwapchain::Frame& frame = swapchain->getFrame(i);
		std::array attachments = {
			frame.imageView,
			depthTexture->imageView,
		};

		VkFramebufferCreateInfo framebufferCraeteInfo = {
			.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
			.renderPass = renderPass,
			.attachmentCount = attachments.size(),
			.pAttachments = attachments.data(),
			.width = extent.width,
			.height = extent.height,
			.layers = 1,
		};

		vkCreateFramebuffer(context->getDevice(), &framebufferCraeteInfo, nullptr, &frames[i].framebuffers);
	}
}

} // namespace arch::gfx::vulkan
