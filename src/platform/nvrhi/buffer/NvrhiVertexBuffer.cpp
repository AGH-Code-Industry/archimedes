#include "NvrhiVertexBuffer.h"

#include "../NvrhiRenderer.h"
#include "NvrhiBufferManager.h"

namespace arch::gfx::nvrhi::buffer {

NvrhiVertexBuffer::NvrhiVertexBuffer(u64 vertexSize, const WeakRef<NvrhiBufferManager>& bufferManager):
	VertexBuffer(vertexSize),
	Buffer(gfx::buffer::BufferType::vertex),
	NvrhiBuffer(gfx::buffer::BufferType::vertex, bufferManager) {}

NvrhiVertexBuffer::~NvrhiVertexBuffer() {}

::nvrhi::BufferDesc NvrhiVertexBuffer::_getDesc(u64 size) const {
	static u64 index = 0;
	return NvrhiBuffer::_getDesc(size)
		.setIsVertexBuffer(true)
		.setInitialState(::nvrhi::ResourceStates::VertexBuffer)
		.setKeepInitialState(true) // enable fully automatic state tracking
		.setDebugName("Vertex Buffer" + std::to_string(index++));
}

} // namespace arch::gfx::nvrhi::buffer
