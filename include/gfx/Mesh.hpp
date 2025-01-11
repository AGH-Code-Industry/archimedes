#pragma once

#include "Mesh.h"
#include "Renderer.h"
#include "buffer/BufferManager.h"

namespace arch::gfx {

template<typename VertexT>
Ref<Mesh> Mesh::create(std::span<VertexT> vertices, std::span<u32> indices) {
	Ref<buffer::BufferManager> bufferManager = Renderer::getCurrent()->getBufferManager();

	Ref<buffer::VertexBuffer> vertexBuffer = bufferManager->createVertexBuffer(vertices);
	Ref<buffer::IndexBuffer> indexBuffer = nullptr; // bufferManager->createIndexBuffer(indices);
	return create(vertexBuffer, indexBuffer);
}

} // namespace arch::gfx
