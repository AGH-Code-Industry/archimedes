#include "platform/nvrhi/NvrhiContext.h"

namespace arch::gfx::nvrhi {

NvrhiContext::~NvrhiContext() {
	_depthStencilBuffer = nullptr;
}

::nvrhi::FramebufferHandle NvrhiContext::getFramebuffer(int index) {
	return _framebuffers[index];
}

void NvrhiContext::_preResizeFramebuffers() {
	_framebuffers.clear();
	_depthStencilBuffer = nullptr;
}

void NvrhiContext::_postResizeFramebuffers() {
	auto backBuffer = _getBackBuffer(0);
	_depthStencilBuffer = getDevice()->createTexture(
		::nvrhi::TextureDesc()
			.setFormat(::nvrhi::Format::D32)
			.setWidth(backBuffer->getDesc().width)
			.setHeight(backBuffer->getDesc().height)
			.setDepth(1)
			.setArraySize(1)
			.setMipLevels(1)
			.setIsRenderTarget(true)
			.setDebugName("DepthStencilBuffer")
	);

	u32 backBufferCount = _getBackBufferCount();
	_framebuffers.resize(backBufferCount);
	for (uint32_t index = 0; index < backBufferCount; index++) {
		_framebuffers[index] = getDevice()->createFramebuffer(
			::nvrhi::FramebufferDesc().addColorAttachment(_getBackBuffer(index)).setDepthAttachment(_depthStencilBuffer)
		);
	}
}

} // namespace arch::gfx::nvrhi
