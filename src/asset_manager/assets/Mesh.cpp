#include <asset_manager/assets/Mesh.h>

namespace arch::assetManager::assets {

Mesh::Mesh(std::span<float> vertices, std::span<uint32_t> indices) {
	Ref<gfx::BufferManager> bufferManager{ gfx::Renderer::getCurrent()->getBufferManager() };
	_vertexBuffer = bufferManager->createVertexBuffer(vertices);
	_indexBuffer = bufferManager->createIndexBuffer(indices);
}

}