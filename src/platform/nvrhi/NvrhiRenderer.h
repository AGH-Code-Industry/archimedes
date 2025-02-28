#pragma once
#include "NvrhiContext.h"
#include "gfx/Renderer.h"
#include <nvrhi/nvrhi.h>

namespace arch::gfx::nvrhi {

namespace buffer {
class NvrhiBufferManager;
}

namespace texture {
class NvrhiTextureManager;
}

namespace pipeline {
class NvrhiPipelineManager;
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

	void usePipeline(const Ref<gfx::pipeline::Pipeline>& pipeline) override;
	void draw(
		const Ref<gfx::buffer::VertexBuffer>& vertexBuffer,
		const Ref<gfx::buffer::IndexBuffer>& indexBuffer,
		const Mat4x4& transform
	) override;

public:
	Ref<gfx::buffer::BufferManager> getBufferManager() override;
	Ref<gfx::texture::TextureManager> getTextureManager() override;
	Ref<gfx::pipeline::PipelineManager> getPipelineManager() override;

public:
	::nvrhi::DeviceHandle getDevice();

	const Ref<NvrhiContext>& getContext() const { return _context; }

private:
	void _ensureGraphicsState();

private:
	Ref<NvrhiContext> _context;
	::nvrhi::DeviceHandle _validationLayer;

	Ref<buffer::NvrhiBufferManager> _bufferManager;
	Ref<texture::NvrhiTextureManager> _textureManager;
	Ref<pipeline::NvrhiPipelineManager> _pipelineManager;

	::nvrhi::CommandListHandle _commandBuffer;

	bool _currentGraphicsStateValid = false;
	::nvrhi::GraphicsState _currentGraphicsState{};
};

} // namespace arch::gfx::nvrhi
