#pragma once

#include "Ref.h"
#include "RenderingAPI.h"
#include "Window.h"
#include "buffer/BufferManager.h"
#include "texture/TextureManager.h"

namespace arch::gfx {

class Mesh;

class Renderer: public std::enable_shared_from_this<Renderer> {
public:
	static Ref<Renderer> create(RenderingAPI api);

	static Ref<Renderer> current() { return s_current; }

protected:
	Renderer() = default;

public:
	virtual ~Renderer() = default;

	static Ref<Renderer> getCurrent();

	void makeCurrent();

	virtual void init(const Ref<Window>& window) = 0;
	virtual void shutdown() = 0;

public:
	// virtual void onResize() = 0;
	// virtual void waitIdle() = 0;

	virtual void prepareFrame() = 0;

	virtual void beginFrame() = 0;
	virtual void endFrame() = 0;

	virtual void present() = 0;

public:
	virtual void setClearColor(Color color) = 0;

	virtual void setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;
	virtual void setScissor(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

	virtual void render(const Ref<Mesh>& mesh, const Mat4x4& transform) = 0;

public:
	virtual Ref<buffer::BufferManager> getBufferManager() = 0;
	virtual Ref<texture::TextureManager> getTextureManager() = 0;

protected:
	Ref<Window> _window = nullptr;

private:
	static Ref<Renderer> s_current;
};

} // namespace arch::gfx
