#pragma once
#include "BufferManager.h"

namespace arch::gfx::buffer {

template<typename VertexT>
Ref<VertexBuffer> BufferManager::createVertexBuffer(std::span<VertexT> vertices) {
	return _createVertexBufferImpl(vertices.data(), vertices.size_bytes());
}

} // namespace arch::gfx::buffer
