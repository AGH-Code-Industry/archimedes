#pragma once

#include <span>

#include <archimedes/Mmath.h>
#include <archimedes/Ref.h>
#include <archimedes/gfx/Buffer.h>

namespace arch::asset::mesh {

// todo: make it an resource/asset
class Mesh final {
public:
	Mesh(const Ref<gfx::VertexBuffer>& vertices, const Ref<gfx::IndexBuffer>& indices);
	~Mesh();

public:
	template<typename VertexT>
	static Ref<Mesh> create(std::span<VertexT> vertices, std::span<u32> indices);
	static Ref<Mesh> create(Ref<gfx::VertexBuffer> vertices, Ref<gfx::IndexBuffer> indices);

public:
	Mesh(const Mesh&) = delete;
	Mesh& operator=(const Mesh&) = delete;

	Mesh(Mesh&& other) noexcept = default;
	Mesh& operator=(Mesh&& other) noexcept = default;

	const Ref<gfx::VertexBuffer>& getVertexBuffer() const { return _vertexBuffer; }

	const Ref<gfx::IndexBuffer>& getIndexBuffer() const { return _indexBuffer; }

private:
	Ref<gfx::VertexBuffer> _vertexBuffer;
	Ref<gfx::IndexBuffer> _indexBuffer;

	friend class Renderer;
};

} // namespace arch::asset::mesh

#include "Mesh.hpp"
