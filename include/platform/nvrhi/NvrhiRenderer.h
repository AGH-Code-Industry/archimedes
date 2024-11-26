#pragma once
#include "NvrhiContext.h"
#include "gfx/Renderer.h"
#include "nvrhi/nvrhi.h"

namespace arch::gfx::nvrhi {

namespace buffer {
class NvrhiBufferManager;
}

namespace texture {
class NvrhiTextureManager;
}

class NvrhiRenderer final: public Renderer {
public:
	NvrhiRenderer(RenderingAPI api, bool debug = true);
	~NvrhiRenderer() override;

	void init(const Ref<Window>& window) override;
	void shutdown() override;

public:
	void onResize(u32 width, u32 height) override;

	void beginFrame() override;
	void present() override;

	void render(const Ref<Mesh>& mesh, const Mat4x4& transform) override;

public:
	Ref<gfx::buffer::BufferManager> getBufferManager() override;
	Ref<gfx::texture::TextureManager> getTextureManager() override;

public:
	::nvrhi::DeviceHandle getDevice();

private:
	Ref<NvrhiContext> _context;
	::nvrhi::DeviceHandle _validationLayer;

	Ref<buffer::NvrhiBufferManager> _bufferManager;
	Ref<texture::NvrhiTextureManager> _textureManager;
};

} // namespace arch::gfx::nvrhi
