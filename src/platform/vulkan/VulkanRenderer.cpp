#include "platform/vulkan/VulkanRenderer.h"

#include <shaderc/shaderc.hpp>

#include "Logger.h"
#include "Window.h"
#include "platform/vulkan/VulkanUtils.h"
#include "platform/vulkan/buffer/VulkanBufferManager.h"
#include "platform/vulkan/texture/VulkanTexture.h"
#include "platform/vulkan/texture/VulkanTextureManager.h"

namespace arch::gfx::vulkan {

constexpr std::string_view vertexShader = R"(
#version 450

layout(location = 0) out vec3 fragColor;

vec2 positions[3] = vec2[](
    vec2(0.0, -0.5),
    vec2(0.5, 0.5),
    vec2(-0.5, 0.5)
);

vec3 colors[3] = vec3[](
    vec3(1.0, 1.0, 1.0),
    vec3(1.0, 1.0, 1.0),
    vec3(1.0, 1.0, 1.0)
);

void main() {
    gl_Position = vec4(positions[gl_VertexIndex], 0.0, 1.0);
    fragColor = colors[gl_VertexIndex];
}
)";

constexpr std::string_view fragmentShader = R"(
#version 450

layout(location = 0) in vec3 fragColor;

layout(location = 0) out vec4 outColor;

void main() {
    outColor = vec4(fragColor, 1.0);
}
)";

VkShaderModule vertModule;
VkShaderModule fragModule;

VkPipeline pipeline;
VkPipelineLayout pipelineLayout;
VkRenderPass renderPass;

VkCommandPool commandPool;

VkShaderModule createShaderModule(VkDevice device, const shaderc::SpvCompilationResult& code) {
	VkShaderModuleCreateInfo createInfo = {
		.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
		.codeSize = (u64)std::distance(code.begin(), code.end()) * sizeof(u32),
		.pCode = code.cbegin(),
	};

	VkShaderModule shaderModule;
	VulkanUtils::vkAssert(
		vkCreateShaderModule(device, &createInfo, nullptr, &shaderModule),
		"Failed to create shader module."
	);

	return shaderModule;
}

void VulkanRenderer::init(const Ref<Window>& window) {
	_window = window;

	Ref<VulkanRenderer> renderer = std::static_pointer_cast<VulkanRenderer>(shared_from_this());

	context = createRef<VulkanContext>();

	swapchain = createRef<VulkanSwapchain>(context, window);

	context->initDevice(swapchain->getSurface());

	swapchain->updateSwapchain();

	_bufferManager = createRef<buffer::VulkanBufferManager>(context);
	_textureManager = createRef<texture::VulkanTextureManager>(context);

	_createDepthTexture();

	/* Render Pass */ {
		// std::vector<VkAttachmentDescription> attachments{
		// 	{
		// 		.format = swapchain->getFormat(),
		// 		.samples = VK_SAMPLE_COUNT_1_BIT,
		// 		.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
		// 		.storeOp = VK_ATTACHMENT_STORE_OP_STORE,
		// 		.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
		// 		.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
		// 		.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
		// 		.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR,
		// 	 },
		//
		// 	{
		// 		.format = VulkanUtils::getFormat(depthTexture->getFormat()),
		// 		.samples = VK_SAMPLE_COUNT_1_BIT,
		// 		.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR,
		// 		.storeOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
		// 		.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE,
		// 		.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE,
		// 		.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED,
		// 		.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL,
		// 	 }
		// };
		//
		// VkAttachmentReference colorAttachmentRef = { .attachment = 0,
		// 											 .layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL };
		// VkAttachmentReference depthAttachmentRef = { .attachment = 1,
		// 											 .layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL };
		//
		// VkSubpassDescription subpass = {
		// 	.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
		// 	.colorAttachmentCount = 1,
		// 	.pColorAttachments = &colorAttachmentRef,
		// 	.pDepthStencilAttachment = &depthAttachmentRef,
		// };
		//
		// std::vector<VkSubpassDependency> dependencies{
		// 	{
		// 		.srcSubpass = VK_SUBPASS_EXTERNAL,
		// 		.dstSubpass = 0,
		// 		.srcStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
		// 		.dstStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
		// 		.srcAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT,
		// 		.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT |
		// 		VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT,.dependencyFlags = 0,
		// 	 },
		//
		// 	{
		// 		.srcSubpass = 0,
		// 		.dstSubpass = VK_SUBPASS_EXTERNAL,
		// 		.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
		// 		.dstStageMask = VK_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
		// 		.srcAccessMask = 0,
		// 		.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | VK_ACCESS_COLOR_ATTACHMENT_READ_BIT,
		// 		.dependencyFlags = 0,
		// 	 }
		// };
		//
		// VkRenderPassCreateInfo createInfo = {
		// 	.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO,
		// 	.attachmentCount = (u32)attachments.size(),
		// 	.pAttachments = attachments.data(),
		// 	.subpassCount = 1,
		// 	.pSubpasses = &subpass,
		// 	.dependencyCount = (u32)dependencies.size(),
		// 	.pDependencies = dependencies.data(),
		//
		// };
		// VulkanUtils::vkAssert(
		// 	vkCreateRenderPass(context->getDevice(), &createInfo, context->getAllocator(), &renderPass),
		// 	"Failed to create render pass."
		// );

		VkAttachmentDescription colorAttachment{};
		colorAttachment.format = swapchain->getFormat();
		colorAttachment.samples = VK_SAMPLE_COUNT_1_BIT;
		colorAttachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
		colorAttachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
		colorAttachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
		colorAttachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
		colorAttachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
		colorAttachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;

		VkAttachmentReference colorAttachmentRef{};
		colorAttachmentRef.attachment = 0;
		colorAttachmentRef.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

		VkSubpassDescription subpass{};
		subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
		subpass.colorAttachmentCount = 1;
		subpass.pColorAttachments = &colorAttachmentRef;

		VkSubpassDependency dependency{};
		dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
		dependency.dstSubpass = 0;
		dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.srcAccessMask = 0;
		dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
		dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

		VkRenderPassCreateInfo renderPassInfo{};
		renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
		renderPassInfo.dependencyCount = 1;
		renderPassInfo.pDependencies = &dependency;
		renderPassInfo.attachmentCount = 1;
		renderPassInfo.pAttachments = &colorAttachment;
		renderPassInfo.subpassCount = 1;
		renderPassInfo.pSubpasses = &subpass;

		if (vkCreateRenderPass(context->getDevice(), &renderPassInfo, nullptr, &renderPass) != VK_SUCCESS) {
			throw std::runtime_error("failed to create render pass!");
		}
	}

	/* Pipeline (Shader/Material) */ {
		shaderc::CompileOptions options;
		shaderc::Compiler compiler;
		vertModule = createShaderModule(
			context->getDevice(),
			compiler.CompileGlslToSpv(
				vertexShader.data(),
				vertexShader.size(),
				shaderc_glsl_vertex_shader,
				"shader.vert.spv",
				options
			)
		);
		fragModule = createShaderModule(
			context->getDevice(),
			compiler.CompileGlslToSpv(
				fragmentShader.data(),
				fragmentShader.size(),
				shaderc_glsl_fragment_shader,
				"shader.frag.spv",
				options
			)
		);

		VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
		vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
		vertShaderStageInfo.module = vertModule;
		vertShaderStageInfo.pName = "main";

		VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
		fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
		fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
		fragShaderStageInfo.module = fragModule;
		fragShaderStageInfo.pName = "main";

		VkPipelineShaderStageCreateInfo shaderStages[] = { vertShaderStageInfo, fragShaderStageInfo };

		std::vector dynamicStates = { VK_DYNAMIC_STATE_VIEWPORT, VK_DYNAMIC_STATE_SCISSOR };

		VkPipelineDynamicStateCreateInfo dynamicState{};
		dynamicState.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
		dynamicState.dynamicStateCount = static_cast<uint32_t>(dynamicStates.size());
		dynamicState.pDynamicStates = dynamicStates.data();

		VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
		vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
		vertexInputInfo.vertexBindingDescriptionCount = 0;
		vertexInputInfo.pVertexBindingDescriptions = nullptr; // Optional
		vertexInputInfo.vertexAttributeDescriptionCount = 0;
		vertexInputInfo.pVertexAttributeDescriptions = nullptr; // Optional

		VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
		inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
		inputAssembly.topology = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST;
		inputAssembly.primitiveRestartEnable = VK_FALSE;

		VkExtent2D swapChainExtent = swapchain->getExtent();

		VkViewport viewport{};
		viewport.x = 0.0f;
		viewport.y = 0.0f;
		viewport.width = (float)swapChainExtent.width;
		viewport.height = (float)swapChainExtent.height;
		viewport.minDepth = 0.0f;
		viewport.maxDepth = 1.0f;

		VkRect2D scissor{};
		scissor.offset = { 0, 0 };
		scissor.extent = swapChainExtent;

		VkPipelineViewportStateCreateInfo viewportState{};
		viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
		viewportState.viewportCount = 1;
		viewportState.pViewports = &viewport;
		viewportState.scissorCount = 1;
		viewportState.pScissors = &scissor;

		VkPipelineRasterizationStateCreateInfo rasterizer{};
		rasterizer.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
		rasterizer.depthClampEnable = VK_FALSE;
		rasterizer.rasterizerDiscardEnable = VK_FALSE;
		rasterizer.polygonMode = VK_POLYGON_MODE_FILL;
		rasterizer.lineWidth = 1.0f;
		rasterizer.cullMode = VK_CULL_MODE_BACK_BIT;
		rasterizer.frontFace = VK_FRONT_FACE_CLOCKWISE;
		rasterizer.depthBiasEnable = VK_FALSE;
		rasterizer.depthBiasConstantFactor = 0.0f; // Optional
		rasterizer.depthBiasClamp = 0.0f; // Optional
		rasterizer.depthBiasSlopeFactor = 0.0f; // Optional

		VkPipelineMultisampleStateCreateInfo multisampling{};
		multisampling.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
		multisampling.sampleShadingEnable = VK_FALSE;
		multisampling.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
		multisampling.minSampleShading = 1.0f; // Optional
		multisampling.pSampleMask = nullptr; // Optional
		multisampling.alphaToCoverageEnable = VK_FALSE; // Optional
		multisampling.alphaToOneEnable = VK_FALSE; // Optional

		VkPipelineColorBlendAttachmentState colorBlendAttachment{};
		colorBlendAttachment.colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
			VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
		colorBlendAttachment.blendEnable = VK_TRUE;
		colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
		colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
		colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
		colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
		colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
		colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

		VkPipelineColorBlendStateCreateInfo colorBlending{};
		colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
		colorBlending.logicOpEnable = VK_FALSE;
		colorBlending.logicOp = VK_LOGIC_OP_COPY; // Optional
		colorBlending.attachmentCount = 1;
		colorBlending.pAttachments = &colorBlendAttachment;
		colorBlending.blendConstants[0] = 0.0f; // Optional
		colorBlending.blendConstants[1] = 0.0f; // Optional
		colorBlending.blendConstants[2] = 0.0f; // Optional
		colorBlending.blendConstants[3] = 0.0f; // Optional

		VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
		pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
		pipelineLayoutInfo.setLayoutCount = 0; // Optional
		pipelineLayoutInfo.pSetLayouts = nullptr; // Optional
		pipelineLayoutInfo.pushConstantRangeCount = 0; // Optional
		pipelineLayoutInfo.pPushConstantRanges = nullptr; // Optional

		if (vkCreatePipelineLayout(context->getDevice(), &pipelineLayoutInfo, nullptr, &pipelineLayout) != VK_SUCCESS) {
			throw std::runtime_error("failed to create pipeline layout!");
		}

		VkGraphicsPipelineCreateInfo pipelineInfo{};
		pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
		pipelineInfo.stageCount = 2;
		pipelineInfo.pStages = shaderStages;
		pipelineInfo.pVertexInputState = &vertexInputInfo;
		pipelineInfo.pInputAssemblyState = &inputAssembly;
		pipelineInfo.pViewportState = &viewportState;
		pipelineInfo.pRasterizationState = &rasterizer;
		pipelineInfo.pMultisampleState = &multisampling;
		pipelineInfo.pDepthStencilState = nullptr; // Optional
		pipelineInfo.pColorBlendState = &colorBlending;
		pipelineInfo.pDynamicState = &dynamicState;
		pipelineInfo.layout = pipelineLayout;
		pipelineInfo.renderPass = renderPass;
		pipelineInfo.subpass = 0;
		pipelineInfo.basePipelineHandle = VK_NULL_HANDLE; // Optional
		pipelineInfo.basePipelineIndex = -1; // Optional

		if (vkCreateGraphicsPipelines(context->getDevice(), VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &pipeline) !=
			VK_SUCCESS) {
			throw std::runtime_error("failed to create graphics pipeline!");
		}
	}

	_createFrames();

	/* Command Buffers */ {
		VkCommandPoolCreateInfo poolInfo{};
		poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
		poolInfo.flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT;
		poolInfo.queueFamilyIndex = context->getQueue(VulkanContext::QueueType::graphics).index;

		if (vkCreateCommandPool(context->getDevice(), &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
			throw std::runtime_error("failed to create command pool!");
		}

		VkCommandBufferAllocateInfo allocInfo{};
		allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
		allocInfo.commandPool = commandPool;
		allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
		allocInfo.commandBufferCount = 1;

		for (auto& frame : frames) {
			if (vkAllocateCommandBuffers(context->getDevice(), &allocInfo, &frame.commandBuffer) != VK_SUCCESS) {
				throw std::runtime_error("failed to allocate command buffers!");
			}
		}
	}

	Logger::info("Created Vulkan instance.");
}

void VulkanRenderer::shutdown() {
	if (swapchain.use_count() > 1) {
		Logger::warn("Swapchain is still in use.");
	}
	swapchain.reset();

	depthTexture.reset();

	_bufferManager.reset();
	_textureManager.reset();

	if (context.use_count() > 1) {
		Logger::warn("Context is still in use.");
	}
	context.reset();
}

Ref<BufferManager> VulkanRenderer::getBufferManager() {
	return _bufferManager;
}

Ref<TextureManager> VulkanRenderer::getTextureManager() {
	return _textureManager;
}

void VulkanRenderer::render(const Ref<Mesh>& mesh, const Mat4x4& transform) {}

void VulkanRenderer::_createDepthTexture() {
	// Depthbuffer
	VkFormat format = context->findSupportedFormat(
		{ VK_FORMAT_D32_SFLOAT, VK_FORMAT_D32_SFLOAT_S8_UINT, VK_FORMAT_D24_UNORM_S8_UINT },
		VK_IMAGE_TILING_OPTIMAL,
		VK_FORMAT_FEATURE_DEPTH_STENCIL_ATTACHMENT_BIT
	);

	VkExtent2D extent = swapchain->getExtent();

	depthTexture = std::static_pointer_cast<texture::VulkanTexture>(
		_textureManager->createTexture2D(extent.width, extent.height, nullptr, VulkanUtils::getFormat(format))
	);
}

void VulkanRenderer::_createFrames() {
	VkExtent2D extent = swapchain->getExtent();

	frames.resize(swapchain->getFrameCount());
	for (int i = 0; i < frames.size(); ++i) {
		const VulkanSwapchain::Frame& frame = swapchain->getFrame(i);
		std::array attachments = { frame.imageView /*, depthTexture->getImage().getImageView()*/ };

		VkFramebufferCreateInfo framebufferCraeteInfo = {
			.sType = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO,
			.renderPass = renderPass,
			.attachmentCount = attachments.size(),
			.pAttachments = attachments.data(),
			.width = extent.width,
			.height = extent.height,
			.layers = 1,
		};

		vkCreateFramebuffer(context->getDevice(), &framebufferCraeteInfo, nullptr, &frames[i].framebuffer);

		VkSemaphoreCreateInfo semaphoreInfo{};
		semaphoreInfo.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;
		VkFenceCreateInfo fenceInfo{};
		fenceInfo.sType = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
		fenceInfo.flags = VK_FENCE_CREATE_SIGNALED_BIT;

		if (vkCreateSemaphore(context->getDevice(), &semaphoreInfo, nullptr, &frames[i].imageAvailableSemaphore) !=
				VK_SUCCESS ||
			vkCreateSemaphore(context->getDevice(), &semaphoreInfo, nullptr, &frames[i].renderFinishedSemaphore) !=
				VK_SUCCESS ||
			vkCreateFence(context->getDevice(), &fenceInfo, nullptr, &frames[i].inFlightFence) != VK_SUCCESS) {
			throw std::runtime_error("failed to create semaphores!");
		}
	}
} // namespace arch::gfx::vulkan

void VulkanRenderer::onResize(u32 width, u32 height) {}

u32 imageIndex;

void VulkanRenderer::beginFrame() {
	auto& frame = frames[_frameIndex];

	vkWaitForFences(context->getDevice(), 1, &frame.inFlightFence, VK_TRUE, UINT64_MAX);
	vkResetFences(context->getDevice(), 1, &frame.inFlightFence);

	vkAcquireNextImageKHR(
		context->getDevice(),
		swapchain->getSwapchain(),
		UINT64_MAX,
		frame.imageAvailableSemaphore,
		VK_NULL_HANDLE,
		&imageIndex
	);

	vkResetCommandBuffer(frame.commandBuffer, 0);

	VkCommandBufferBeginInfo beginInfo{};
	beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
	beginInfo.flags = 0; // Optional
	beginInfo.pInheritanceInfo = nullptr; // Optional

	if (vkBeginCommandBuffer(frame.commandBuffer, &beginInfo) != VK_SUCCESS) {
		throw std::runtime_error("failed to begin recording command buffer!");
	}

	VkRenderPassBeginInfo renderPassInfo{};
	renderPassInfo.sType = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
	renderPassInfo.renderPass = renderPass;
	renderPassInfo.framebuffer = frame.framebuffer;
	renderPassInfo.renderArea.offset = { 0, 0 };
	renderPassInfo.renderArea.extent = swapchain->getExtent();
	VkClearValue clearColor = { { { 0.0f, 0.0f, 0.0f, 1.0f } } };
	renderPassInfo.clearValueCount = 1;
	renderPassInfo.pClearValues = &clearColor;
	vkCmdBeginRenderPass(frame.commandBuffer, &renderPassInfo, VK_SUBPASS_CONTENTS_INLINE);

	vkCmdBindPipeline(frame.commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline);

	VkViewport viewport{};
	viewport.x = 0.0f;
	viewport.y = 0.0f;
	viewport.width = static_cast<float>(swapchain->getExtent().width);
	viewport.height = static_cast<float>(swapchain->getExtent().height);
	viewport.minDepth = 0.0f;
	viewport.maxDepth = 1.0f;
	vkCmdSetViewport(frame.commandBuffer, 0, 1, &viewport);

	VkRect2D scissor{};
	scissor.offset = { 0, 0 };
	scissor.extent = swapchain->getExtent();
	vkCmdSetScissor(frame.commandBuffer, 0, 1, &scissor);

	vkCmdDraw(frame.commandBuffer, 3, 1, 0, 0);
}

void VulkanRenderer::present() {
	auto& frame = frames[_frameIndex];

	vkCmdEndRenderPass(frame.commandBuffer);

	if (vkEndCommandBuffer(frame.commandBuffer) != VK_SUCCESS) {
		throw std::runtime_error("failed to record command buffer!");
	}

	VkSubmitInfo submitInfo{};
	submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
	VkSemaphore waitSemaphores[] = { frame.imageAvailableSemaphore };
	VkPipelineStageFlags waitStages[] = { VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT };
	submitInfo.waitSemaphoreCount = 1;
	submitInfo.pWaitSemaphores = waitSemaphores;
	submitInfo.pWaitDstStageMask = waitStages;
	submitInfo.commandBufferCount = 1;
	submitInfo.pCommandBuffers = &frame.commandBuffer;

	VkSemaphore signalSemaphores[] = { frame.renderFinishedSemaphore };
	submitInfo.signalSemaphoreCount = 1;
	submitInfo.pSignalSemaphores = signalSemaphores;

	if (vkQueueSubmit(
			context->getQueue(VulkanContext::QueueType::graphics).queue,
			1,
			&submitInfo,
			frame.inFlightFence
		) != VK_SUCCESS) {
		throw std::runtime_error("failed to submit draw command buffer!");
	}

	VkPresentInfoKHR presentInfo{};
	presentInfo.sType = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;

	presentInfo.waitSemaphoreCount = 1;
	presentInfo.pWaitSemaphores = signalSemaphores;

	VkSwapchainKHR swapChains[] = { swapchain->getSwapchain() };
	presentInfo.swapchainCount = 1;
	presentInfo.pSwapchains = swapChains;
	presentInfo.pImageIndices = &imageIndex;
	presentInfo.pResults = nullptr; // Optional

	vkQueuePresentKHR(context->getQueue(VulkanContext::QueueType::presentaion).queue, &presentInfo);

	_frameIndex = (_frameIndex + 1) % frames.size();
}

} // namespace arch::gfx::vulkan
