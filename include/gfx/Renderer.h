#pragma once

#include "Ref.h"
#include "RenderingAPI.h"
#include "Window.h"
#include "buffer/BufferManager.h"
#include "pipeline/Pipeline.h"
#include "pipeline/PipelineManager.h"
#include "texture/TextureManager.h"

namespace arch::gfx {

class Renderer: public std::enable_shared_from_this<Renderer> {
public:
	static Ref<Renderer> create(RenderingAPI api);

	static Ref<Renderer> current() { return s_current; }

protected:
	explicit Renderer(RenderingAPI api, bool debug = true): _api(api), _debug(debug) {}

public:
	virtual ~Renderer() = default;

	static Ref<Renderer> getCurrent();

	void makeCurrent();

	virtual void init(const Ref<Window>& window) = 0;
	virtual void shutdown() = 0;

public:
	void setClearColor(Color color);

public:
	virtual void onResize(u32 width, u32 height) = 0;

	virtual bool beginFrame() = 0;
	virtual void present() = 0;

public:

	virtual void usePipeline(const Ref<pipeline::Pipeline>& pipeline) = 0;

	virtual void draw(
		const Ref<buffer::VertexBuffer>& vertexBuffer,
		const Ref<buffer::IndexBuffer>& indexBuffer,
		const Mat4x4& transform
	) = 0;

public:
	virtual Ref<buffer::BufferManager> getBufferManager() = 0;
	virtual Ref<texture::TextureManager> getTextureManager() = 0;
	virtual Ref<pipeline::PipelineManager> getPipelineManager() = 0;
	Ref<Window> getWindow();

protected:
	Ref<Window> _window = nullptr;
	RenderingAPI _api = RenderingAPI::none;
	bool _debug = false;

	Color _clearColor = { 0, 0, 0, 0 };

private:
	static Ref<Renderer> s_current;
};

} // namespace arch::gfx
