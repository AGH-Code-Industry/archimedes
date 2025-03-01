#include "gfx/buffer/BufferManager.h"

#include "exceptions/InvalidEnumException.h"

namespace arch::gfx::buffer {

Ref<VertexBuffer> BufferManager::createVertexBuffer(u64 vertexSize) {
	return createVertexBuffer(nullptr, 0, vertexSize);
}

Ref<VertexBuffer> BufferManager::createVertexBuffer(void* data, u32 size, u64 vertexSize) {
	return _createVertexBufferImpl(data, size, vertexSize);
}

Ref<IndexBuffer> BufferManager::createIndexBuffer() {
	return createIndexBuffer(std::span((u32*)nullptr, 0));
}

Ref<IndexBuffer> BufferManager::createIndexBuffer(std::span<u32> indices) {
	return _createIndexBufferImpl(indices);
}

Ref<Buffer> BufferManager::createBuffer(BufferType type) {
	return createBuffer(type, nullptr, 0);
}

Ref<Buffer> BufferManager::createBuffer(BufferType type, void* data, u64 size) {
	if (type == BufferType::vertex) {
		throw InvalidEnumException("BufferType::vertex is not supported. Use createVertexBuffer instead.");
	}

	if (type == BufferType::index) {
		return createIndexBuffer(std::span((u32*)data, size / sizeof(u32)));
	}

	return _createBufferImpl(type, data, size);
}

} // namespace arch::gfx::buffer
