#pragma once
#include "NvrhiContext.h"
#include "gfx/Renderer.h"
#include "nvrhi/nvrhi.h"

namespace arch::gfx::nvrhi {

class NvrhiRenderer final: public Renderer {
public:
	NvrhiRenderer(RenderingAPI api, bool debug = true);
	~NvrhiRenderer() override = default;

	void init(const Ref<Window>& window) override;
	void shutdown() override;

public:
	void prepareFrame() override;
	void beginFrame() override;
	void endFrame() override;
	void present() override;
	void setClearColor(Color color) override;
	void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
	void setScissor(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;
	void render(const Ref<Mesh>& mesh, const Mat4x4& transform) override;

public:
	Ref<buffer::BufferManager> getBufferManager() override;
	Ref<texture::TextureManager> getTextureManager() override;

public:
	::nvrhi::DeviceHandle getDevice();

private:
	Ref<NvrhiContext> _context;
	::nvrhi::DeviceHandle _validationLayer;
};

} // namespace arch::gfx::nvrhi
