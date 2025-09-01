#include <archimedes/asset/mesh/Mesh.h>

namespace arch::asset::mesh {

Mesh::Mesh(const Ref<gfx::VertexBuffer>& vertexBuffer, const Ref<gfx::IndexBuffer>& indexBuffer):
	_vertexBuffer(vertexBuffer),
	_indexBuffer(indexBuffer) {}

Mesh::~Mesh() {}

Ref<Mesh> Mesh::create(Ref<gfx::VertexBuffer> vertexBuffer, Ref<gfx::IndexBuffer> indexBuffer) {
	return createRef<Mesh>(vertexBuffer, indexBuffer);
}

} // namespace arch::asset::mesh
