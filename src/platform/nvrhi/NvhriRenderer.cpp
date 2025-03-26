#include "Logger.h"
#include "NvrhiRenderer.h"
#include "buffer/NvrhiBufferManager.h"
#include "buffer/NvrhiIndexBuffer.h"
#include "buffer/NvrhiVertexBuffer.h"
#include "context/NvrhiVulkanContext.h"
#include "nvrhi/utils.h"
#include "nvrhi/validation.h"
#include "pipeline/NvrhiPipeline.h"
#include "pipeline/NvrhiPipelineManager.h"
#include "texture/NvrhiTextureManager.h"

namespace arch::gfx::nvrhi {

NvrhiRenderer::NvrhiRenderer(RenderingAPI api, bool debug): Renderer(api, debug) {}

NvrhiRenderer::~NvrhiRenderer() {
	if (_context) {
		shutdown();
	}
}

void NvrhiRenderer::init(const Ref<Window>& window) {
	_window = window;
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

	WeakRef renderer = std::static_pointer_cast<NvrhiRenderer>(shared_from_this());
	_bufferManager = createRef<buffer::NvrhiBufferManager>(renderer);
	_textureManager = createRef<texture::NvrhiTextureManager>(renderer);
	_pipelineManager = createRef<pipeline::NvrhiPipelineManager>(renderer);

	_commandBuffer = getDevice()->createCommandList();
}

void NvrhiRenderer::shutdown() {
	_bufferManager.reset();
	_textureManager.reset();

	_commandBuffer = nullptr;

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

bool NvrhiRenderer::beginFrame() {
	getDevice()->runGarbageCollection();

	if (!_context->beginFrame()) {
		return false;
	}

	_commandBuffer->open();

	::nvrhi::FramebufferHandle currentFramebuffer = _context->getFramebuffer(_context->getCurrentFrameIndex());
	::nvrhi::utils::ClearColorAttachment(
		_commandBuffer,
		currentFramebuffer,
		0,
		::nvrhi::Color(_clearColor.r, _clearColor.g, _clearColor.b, _clearColor.a)
	);

	uint2 framebufferSize = _context->getFramebufferSize();

	_currentGraphicsStateValid = false;
	_currentGraphicsState.setFramebuffer(currentFramebuffer)
		.setViewport(
			::nvrhi::ViewportState().addViewportAndScissorRect(::nvrhi::Viewport(framebufferSize.x, framebufferSize.y))
		);

	return true;
}

void NvrhiRenderer::present() {
	_commandBuffer->close();
	getDevice()->executeCommandList(_commandBuffer);

	_context->present();
}

void NvrhiRenderer::usePipeline(const Ref<gfx::pipeline::Pipeline>& pipeline) {
	Ref<pipeline::NvrhiPipeline> p = std::static_pointer_cast<pipeline::NvrhiPipeline>(pipeline);
	if (_currentGraphicsState.pipeline != p->getNativeHandle()) {
		_currentGraphicsState.pipeline = p->getNativeHandle();

		_currentGraphicsState.bindings.resize(0);
		_currentGraphicsState.addBindingSet(p->getNativeBindingSet());

		_currentGraphicsStateValid = false;
	}
}

void NvrhiRenderer::draw(
	const Ref<gfx::buffer::VertexBuffer>& vertexBuffer,
	const Ref<gfx::buffer::IndexBuffer>& indexBuffer,
	const Mat4x4& transform
) {
	Ref<buffer::NvrhiVertexBuffer> vb = std::static_pointer_cast<buffer::NvrhiVertexBuffer>(vertexBuffer);
	::nvrhi::VertexBufferBinding vbBinding = ::nvrhi::VertexBufferBinding().setBuffer(vb->getNativeHandle());

	Ref<buffer::NvrhiIndexBuffer> ib = std::static_pointer_cast<buffer::NvrhiIndexBuffer>(indexBuffer);
	::nvrhi::IndexBufferBinding ibBinding = ::nvrhi::IndexBufferBinding().setBuffer(ib->getNativeHandle());

	_currentGraphicsState.vertexBuffers.resize(0);
	_currentGraphicsState.addVertexBuffer(vbBinding).setIndexBuffer(ibBinding);

	_currentGraphicsStateValid = false;

	_ensureGraphicsState();

	// todo: fix this
	_commandBuffer->setPushConstants(&transform, 128);

	auto drawArguments = ::nvrhi::DrawArguments().setVertexCount(ib->getIndexCount());

	_commandBuffer->drawIndexed(drawArguments);
}

Ref<gfx::buffer::BufferManager> NvrhiRenderer::getBufferManager() {
	return _bufferManager;
}

Ref<gfx::texture::TextureManager> NvrhiRenderer::getTextureManager() {
	return _textureManager;
}

Ref<gfx::pipeline::PipelineManager> NvrhiRenderer::getPipelineManager() {
	return _pipelineManager;
}

::nvrhi::DeviceHandle NvrhiRenderer::getDevice() {
	if (_validationLayer) {
		return _validationLayer;
	}

	return _context->getDevice();
}

void NvrhiRenderer::_ensureGraphicsState() {
	if (_currentGraphicsStateValid) {
		return;
	}

	_commandBuffer->setGraphicsState(_currentGraphicsState);
	_currentGraphicsStateValid = true;
}

} // namespace arch::gfx::nvrhi
