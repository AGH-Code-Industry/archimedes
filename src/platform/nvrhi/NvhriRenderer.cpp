#include <shaderc/shaderc.hpp>

#include "Logger.h"
#include "NvrhiRenderer.h"
#include "buffer/NvrhiBufferManager.h"
#include "context/NvrhiVulkanContext.h"
#include "nvrhi/utils.h"
#include "nvrhi/validation.h"
#include "texture/NvrhiTextureManager.h"

namespace arch::gfx::nvrhi {

struct Vertex {
	float3 position;
	float2 uv;
};

static const Vertex g_Vertices[] = {
	{ { -.25f, -.25f, 0.1f }, { 0.f, 0.f } },
	{	  { 0.f, .25f, 0.1f }, { .5f, 1.f } },
	{  { .25f, -.25f, 0.1f }, { 1.f, 0.f } },
};

constexpr std::string_view vertexShader = R"(
#version 450

layout(location = 0) in vec3 inPosition;
layout(location = 1) in vec2 inUV;

layout(location = 0) out vec3 fragColor;

layout(binding = 256) uniform UniformBufferObject {
    mat4 mvp;
} ubo;


void main() {
    gl_Position = ubo.mvp * vec4(inPosition, 1.0);
    fragColor = vec3(inUV, 0.0);
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

::nvrhi::GraphicsPipelineHandle s_pipeline;
::nvrhi::BindingLayoutHandle s_bindingLayout;
::nvrhi::BindingSetHandle s_bindingSet;

::nvrhi::BufferHandle s_constantBuffer;
::nvrhi::BufferHandle s_vertexBuffer;

::nvrhi::CommandListHandle s_commandList;

NvrhiRenderer::NvrhiRenderer(RenderingAPI api, bool debug): Renderer(api, debug) {
	_bufferManager = createRef<buffer::NvrhiBufferManager>();
	// _textureManager = createRef<texture::NvrhiTextureManager>();
}

NvrhiRenderer::~NvrhiRenderer() {
	if (_context) {
		shutdown();
	}
}

void NvrhiRenderer::init(const Ref<Window>& window) {
	switch (_api) {
		case RenderingAPI::Nvrhi_VK:
			{
				_context = createRef<NvrhiVulkanContext>(_debug);
				break;
			}
		// case RenderingAPI::Nvrhi_DX11: break;
		// case RenderingAPI::Nvrhi_DX12: break;
		default: Logger::error("Invalid rendering API"); break;
	}
	_context->init(window);

	if (_debug) {
		_validationLayer = ::nvrhi::validation::createValidationLayer(_context->getDevice());
	}

	{
		shaderc::CompileOptions options;
		shaderc::Compiler compiler;
		auto result = compiler.CompileGlslToSpv(
			vertexShader.data(),
			vertexShader.size(),
			shaderc_glsl_vertex_shader,
			"shader.vert.spv",
			options
		);

		if (result.GetCompilationStatus() != shaderc_compilation_status_success) {
			throw vulkan::exceptions::VulkanException(result.GetErrorMessage());
		}

		::nvrhi::ShaderHandle vertexShader = getDevice()->createShader(
			::nvrhi::ShaderDesc(::nvrhi::ShaderType::Vertex),
			result.begin(),
			(u64)std::distance(result.begin(), result.end()) * sizeof(u32)
		);

		::nvrhi::VertexAttributeDesc attributes[] = { ::nvrhi::VertexAttributeDesc()
														  .setName("POSITION")
														  .setFormat(::nvrhi::Format::RGB32_FLOAT)
														  .setOffset(0)
														  .setElementStride(sizeof(float) * 5),
													  ::nvrhi::VertexAttributeDesc()
														  .setName("TEXCOORD")
														  .setFormat(::nvrhi::Format::RG32_FLOAT)
														  .setOffset(sizeof(float) * 3)
														  .setElementStride(sizeof(float) * 5) };

		::nvrhi::InputLayoutHandle inputLayout =
			getDevice()->createInputLayout(attributes, uint32_t(std::size(attributes)), vertexShader);

		result = compiler.CompileGlslToSpv(
			fragmentShader.data(),
			fragmentShader.size(),
			shaderc_glsl_fragment_shader,
			"shader.vert.spv",
			options
		);
		::nvrhi::ShaderHandle pixelShader = getDevice()->createShader(
			::nvrhi::ShaderDesc(::nvrhi::ShaderType::Pixel),
			result.begin(),
			(u64)std::distance(result.begin(), result.end()) * sizeof(u32)
		);

		auto constantBufferDesc = ::nvrhi::BufferDesc()
									  .setByteSize(sizeof(Mat4x4)) // stores one matrix
									  .setIsConstantBuffer(true)
									  .setIsVolatile(true)
									  .setMaxVersions(16); // number of automatic versions, only necessary on Vulkan

		s_constantBuffer = getDevice()->createBuffer(constantBufferDesc);

		auto vertexBufferDesc = ::nvrhi::BufferDesc()
									.setByteSize(sizeof(g_Vertices))
									.setIsVertexBuffer(true)
									.setInitialState(::nvrhi::ResourceStates::VertexBuffer)
									.setKeepInitialState(true) // enable fully automatic state tracking
									.setDebugName("Vertex Buffer");

		s_vertexBuffer = getDevice()->createBuffer(vertexBufferDesc);

		s_commandList = getDevice()->createCommandList();

		auto bindingSetDesc = ::nvrhi::BindingSetDesc()
								  // .addItem(::nvrhi::BindingSetItem::Texture_SRV(0, geometryTexture))
								  .addItem(::nvrhi::BindingSetItem::ConstantBuffer(0, s_constantBuffer));

		::nvrhi::utils::CreateBindingSetAndLayout(
			getDevice(),
			::nvrhi::ShaderType::All,
			0,
			bindingSetDesc,
			s_bindingLayout,
			s_bindingSet
		);

		auto pipelineDesc = ::nvrhi::GraphicsPipelineDesc()
								.setInputLayout(inputLayout)
								.setVertexShader(vertexShader)
								.setPixelShader(pixelShader)
								.addBindingLayout(s_bindingLayout);

		pipelineDesc.renderState.depthStencilState.depthTestEnable = false;

		s_pipeline = getDevice()->createGraphicsPipeline(pipelineDesc, _context->getFramebuffer(0));

		s_commandList->open();

		s_commandList->writeBuffer(s_vertexBuffer, g_Vertices, sizeof(g_Vertices));

		s_commandList->close();
		getDevice()->executeCommandList(s_commandList);
	}
}

void NvrhiRenderer::shutdown() {
	s_pipeline = nullptr;
	s_bindingLayout = nullptr;
	s_bindingSet = nullptr;

	s_constantBuffer = nullptr;
	s_vertexBuffer = nullptr;
	s_commandList = nullptr;

	_validationLayer = nullptr;

	if (_context.use_count() > 1) {
		Logger::warn("NVRHI context is still in use.");
	}
	_context->shutdown();
	_context.reset();
}

void NvrhiRenderer::onResize(u32 width, u32 height) {
	_context->onResize(width, height);
}

void NvrhiRenderer::beginFrame() {
	_context->beginFrame();

	s_commandList->open();

	::nvrhi::utils::ClearColorAttachment(
		s_commandList,
		_context->getFramebuffer(_context->getCurrentFrameIndex()),
		0,
		::nvrhi::Color(_clearColor.r, _clearColor.g, _clearColor.b, _clearColor.a)
	);
}

void NvrhiRenderer::present() {
	s_commandList->close();
	getDevice()->executeCommandList(s_commandList);

	_context->present();
}

void NvrhiRenderer::render(const Ref<Mesh>& mesh, const Mat4x4& transform) {
	::nvrhi::FramebufferHandle currentFramebuffer = _context->getFramebuffer(_context->getCurrentFrameIndex());

	s_commandList->writeBuffer(s_constantBuffer, &transform, sizeof(transform));

	::nvrhi::VertexBufferBinding vbufBinding =
		::nvrhi::VertexBufferBinding().setBuffer(s_vertexBuffer).setSlot(0).setOffset(0);

	uint2 framebufferSize = _context->getFramebufferSize();
	auto graphicsState = ::nvrhi::GraphicsState()
							 .setPipeline(s_pipeline)
							 .setFramebuffer(currentFramebuffer)
							 .setViewport(
								 ::nvrhi::ViewportState().addViewportAndScissorRect(
									 ::nvrhi::Viewport(framebufferSize.x, framebufferSize.y)
								 )
							 )
							 .addBindingSet(s_bindingSet)
							 .addVertexBuffer(vbufBinding);
	s_commandList->setGraphicsState(graphicsState);

	auto drawArguments = ::nvrhi::DrawArguments().setVertexCount(3);
	s_commandList->draw(drawArguments);
}

Ref<gfx::buffer::BufferManager> NvrhiRenderer::getBufferManager() {
	return _bufferManager;
}

Ref<gfx::texture::TextureManager> NvrhiRenderer::getTextureManager() {
	return _textureManager;
}

::nvrhi::DeviceHandle NvrhiRenderer::getDevice() {
	if (_validationLayer) {
		return _validationLayer;
	}

	return _context->getDevice();
}

} // namespace arch::gfx::nvrhi
