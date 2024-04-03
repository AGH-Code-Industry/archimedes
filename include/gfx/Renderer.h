#pragma once

#include "Ref.h"
#include "RenderingAPI.h"
#include "Window.h"
#include "buffer/BufferManager.h"

namespace arch::gfx {

class Mesh;

class Renderer: public std::enable_shared_from_this<Renderer> {
public:
	static Ref<Renderer> create(RenderingAPI api);

protected:
	Renderer(const Ref<buffer::BufferManager>& bufferManager): _bufferManager(bufferManager) {}

public:
	virtual ~Renderer() = default;

	static Ref<Renderer> getCurrent();

	void makeCurrent();

	virtual void init(const Ref<Window>& window) = 0;
	virtual void shutdown() = 0;

public:
	Ref<buffer::BufferManager> getBufferManager() const { return _bufferManager; }

public:
	virtual void render(const Ref<Mesh>& mesh, const Mat4x4& transform) = 0;

protected:
	Ref<Window> _window = nullptr;

	Ref<buffer::BufferManager> _bufferManager;

private:
	static Ref<Renderer> current;
};

} // namespace arch::gfx
