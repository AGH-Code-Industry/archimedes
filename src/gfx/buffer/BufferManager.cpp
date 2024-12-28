#include "gfx/buffer/BufferManager.h"

namespace arch::gfx::buffer {

Ref<VertexBuffer> BufferManager::createVertexBuffer() {
	return _createVertexBufferImpl(nullptr, 0);
}

Ref<VertexBuffer> BufferManager::createVertexBuffer(void* data, u32 size) {
	return _createVertexBufferImpl(data, size);
}

Ref<IndexBuffer> BufferManager::createIndexBuffer() {
	return _createIndexBufferImpl(std::span((u32*)nullptr, 0));
}

Ref<IndexBuffer> BufferManager::createIndexBuffer(std::span<u32> indices) {
	return _createIndexBufferImpl(indices);
}

Ref<Buffer> BufferManager::createBuffer() {
	return _createBufferImpl(nullptr, 0);
}

} // namespace arch::gfx::buffer
