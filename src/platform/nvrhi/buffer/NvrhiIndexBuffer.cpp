#include "NvrhiIndexBuffer.h"

namespace arch::gfx::nvrhi::buffer {

NvrhiIndexBuffer::NvrhiIndexBuffer(const WeakRef<NvrhiBufferManager>& bufferManager):
	Buffer(gfx::buffer::BufferType::index),
	NvrhiBuffer(gfx::buffer::BufferType::index, bufferManager) {}

NvrhiIndexBuffer::~NvrhiIndexBuffer() {}

::nvrhi::BufferDesc NvrhiIndexBuffer::_getDesc(u64 size) const {
	static u64 index = 0;
	return NvrhiBuffer::_getDesc(size)
		.setIsIndexBuffer(true)
		.setInitialState(::nvrhi::ResourceStates::IndexBuffer)
		.setKeepInitialState(true) // enable fully automatic state tracking
		.setDebugName("Index Buffer " + std::to_string(index++));
}

} // namespace arch::gfx::nvrhi::buffer
