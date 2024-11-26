#include "platform/nvrhi/buffer/NvrhiVertexBuffer.h"

namespace arch::gfx::nvrhi::buffer {

NvrhiVertexBuffer::NvrhiVertexBuffer():
	NvrhiBuffer(gfx::buffer::BufferType::vertex),
	Buffer(gfx::buffer::BufferType::vertex) {}

NvrhiVertexBuffer::~NvrhiVertexBuffer() {}

} // namespace arch::gfx::nvrhi::buffer
