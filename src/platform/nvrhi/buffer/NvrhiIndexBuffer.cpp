#include "NvrhiIndexBuffer.h"

namespace arch::gfx::nvrhi::buffer {

NvrhiIndexBuffer::NvrhiIndexBuffer(const WeakRef<NvrhiBufferManager>& bufferManager):
	Buffer(gfx::buffer::BufferType::index),
	NvrhiBuffer(gfx::buffer::BufferType::index, bufferManager) {}

NvrhiIndexBuffer::~NvrhiIndexBuffer() {}

} // namespace arch::gfx::nvrhi::buffer
