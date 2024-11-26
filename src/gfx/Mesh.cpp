#include "gfx/Mesh.h"

#include "gfx/Buffer.h"

namespace arch::gfx {

Mesh::Mesh(const Ref<VertexBuffer>& vertexBuffer, const Ref<IndexBuffer>& indexBuffer):
	_vertexBuffer(vertexBuffer),
	_indexBuffer(indexBuffer) {}

Mesh::~Mesh() {}

Ref<Mesh> Mesh::create(Ref<VertexBuffer> vertexBuffer, Ref<IndexBuffer> indexBuffer) {
	return createRef<Mesh>(vertexBuffer, indexBuffer);
}

} // namespace arch::gfx
