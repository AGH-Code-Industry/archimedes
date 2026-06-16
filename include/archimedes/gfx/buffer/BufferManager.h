#pragma once

#include <span>

#include "Buffer.h"
#include "IndexBuffer.h"
#include "VertexBuffer.h"

namespace arch::gfx::buffer {

class BufferManager {
public:
	BufferManager() = default;
	virtual ~BufferManager() = default;

public:
	Ref<VertexBuffer> createVertexBuffer(u64 vertexSize);
	template<typename VertexT>
	Ref<VertexBuffer> createVertexBuffer(std::span<VertexT> vertices);
	Ref<VertexBuffer> createVertexBuffer(void* data, u32 size, u64 vertexSize);

	Ref<IndexBuffer> createIndexBuffer();
	Ref<IndexBuffer> createIndexBuffer(std::span<u32> indices);

	Ref<Buffer> createBuffer(BufferType type);
	Ref<Buffer> createBuffer(BufferType type, void* data, u64 size);

protected:
	virtual Ref<VertexBuffer> _createVertexBufferImpl(void* data, u64 size, u64 vertexSize) = 0;
	virtual Ref<IndexBuffer> _createIndexBufferImpl(std::span<u32> indices) = 0;
	virtual Ref<Buffer> _createBufferImpl(BufferType type, void* data, u64 size) = 0;
};

} // namespace arch::gfx::buffer

#include "BufferManager.hpp"
