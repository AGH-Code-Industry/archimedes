#pragma once
#include "BufferManager.h"

namespace arch::gfx::buffer {

template<typename VertexT>
Ref<VertexBuffer> BufferManager::createVertexBuffer(std::span<VertexT> vertices) {
	return _createVertexBufferImpl(vertices.data(), vertices.size_bytes(), sizeof(VertexT));
}

} // namespace arch::gfx::buffer
