#pragma once

#include "Mesh.h"
#include <archimedes/gfx/Renderer.h>
#include <archimedes/gfx/buffer/BufferManager.h>

namespace arch::asset::mesh {

template<typename VertexT>
Ref<Mesh> Mesh::create(std::span<VertexT> vertices, std::span<u32> indices) {
	Ref<gfx::BufferManager> bufferManager = gfx::Renderer::getCurrent()->getBufferManager();

	Ref<gfx::VertexBuffer> vertexBuffer = bufferManager->createVertexBuffer(vertices);
	Ref<gfx::IndexBuffer> indexBuffer = bufferManager->createIndexBuffer(indices);
	return create(vertexBuffer, indexBuffer);
}

} // namespace arch::asset::mesh
