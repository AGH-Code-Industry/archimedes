#pragma once

#include "ArchMath.h"
#include "Ref.h"
#include "nvrhi/nvrhi.h"

namespace arch {
class Window;
}

namespace arch::gfx::nvrhi {

class NvrhiContext {
public:
	NvrhiContext() = default;
	virtual ~NvrhiContext();

	virtual void init(const Ref<Window>& window) = 0;
	virtual void shutdown();

	virtual void onResize(u32 width, u32 height) = 0;

	virtual void beginFrame() = 0;
	virtual void present() = 0;

public:
	::nvrhi::FramebufferHandle getFramebuffer(int index);

public:
	virtual int getCurrentFrameIndex() = 0;
	virtual uint2 getFramebufferSize() const = 0;

	virtual ::nvrhi::DeviceHandle getDevice() = 0;

protected:
	void _preResizeFramebuffers();
	void _postResizeFramebuffers();

protected:
	virtual u32 _getBackBufferCount() const = 0;
	virtual ::nvrhi::TextureHandle _getBackBuffer(uint32_t index) const = 0;

protected:
	bool _vsync = false;
	std::vector<::nvrhi::FramebufferHandle> _framebuffers;

	::nvrhi::TextureHandle _depthStencilBuffer;
};

} // namespace arch::gfx::nvrhi
