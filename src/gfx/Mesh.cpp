#include "gfx/Mesh.h"

#include "gfx/Buffer.h"

namespace arch::gfx {

Mesh::Mesh(const std::vector<float3>& vertices, const std::vector<u32>& indices) {}

Mesh::~Mesh() {}

Ref<Mesh> Mesh::create(Ref<VertexBuffer> vertices, Ref<IndexBuffer> indices) {
	return nullptr;
}

} // namespace arch::gfx
