#include "NvrhiVertexBuffer.h"

#include "../NvrhiRenderer.h"
#include "NvrhiBufferManager.h"

namespace arch::gfx::nvrhi::buffer {

NvrhiVertexBuffer::NvrhiVertexBuffer(const WeakRef<NvrhiBufferManager>& bufferManager):
	Buffer(gfx::buffer::BufferType::vertex),
	NvrhiBuffer(gfx::buffer::BufferType::vertex, bufferManager) {}

NvrhiVertexBuffer::~NvrhiVertexBuffer() {}

::nvrhi::BufferDesc NvrhiVertexBuffer::_getDesc(u64 size) const {
	return NvrhiBuffer::_getDesc(size)
		.setIsVertexBuffer(true)
		.setInitialState(::nvrhi::ResourceStates::VertexBuffer)
		.setKeepInitialState(true) // enable fully automatic state tracking
		.setDebugName("Vertex Buffer");
}

} // namespace arch::gfx::nvrhi::buffer
