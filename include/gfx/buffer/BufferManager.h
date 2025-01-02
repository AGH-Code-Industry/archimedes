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
	Ref<VertexBuffer> createVertexBuffer();
	template<typename VertexT>
	Ref<VertexBuffer> createVertexBuffer(std::span<VertexT> vertices);
	Ref<VertexBuffer> createVertexBuffer(void* data, u32 size);

	Ref<IndexBuffer> createIndexBuffer();
	Ref<IndexBuffer> createIndexBuffer(std::span<u32> indices);

	Ref<Buffer> createBuffer();

protected:
	virtual Ref<VertexBuffer> _createVertexBufferImpl(void* data, u32 size) = 0;
	virtual Ref<IndexBuffer> _createIndexBufferImpl(std::span<u32> indices) = 0;
	virtual Ref<Buffer> _createBufferImpl(void* data, u32 size) = 0;
};

} // namespace arch::gfx::buffer

#include "BufferManager.hpp"
