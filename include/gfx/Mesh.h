#pragma once

#include <span>
#include <vector>

#include "Mmath.h"
#include "Ref.h"
#include "buffer/IndexBuffer.h"
#include "buffer/VertexBuffer.h"

namespace arch::gfx {

class Mesh final {
public:
	Mesh(const Ref<buffer::VertexBuffer>& vertices, const Ref<buffer::IndexBuffer>& indices);
	~Mesh();

public:
	template<typename VertexT>
	static Ref<Mesh> create(std::span<VertexT> vertices, std::span<u32> indices);
	static Ref<Mesh> create(Ref<buffer::VertexBuffer> vertices, Ref<buffer::IndexBuffer> indices);

public:
	Mesh(const Mesh&) = delete;
	Mesh& operator=(const Mesh&) = delete;

	Mesh(Mesh&& other) noexcept = default;
	Mesh& operator=(Mesh&& other) noexcept = default;

private:
	Ref<buffer::VertexBuffer> _vertexBuffer;
	Ref<buffer::IndexBuffer> _indexBuffer;
};

} // namespace arch::gfx

#include "Mesh.hpp"
