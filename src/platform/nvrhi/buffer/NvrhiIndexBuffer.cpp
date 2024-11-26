#include "platform/nvrhi/buffer/NvrhiIndexBuffer.h"

namespace arch::gfx::nvrhi::buffer {

NvrhiIndexBuffer::NvrhiIndexBuffer():
	NvrhiBuffer(gfx::buffer::BufferType::vertex),
	Buffer(gfx::buffer::BufferType::vertex) {}

NvrhiIndexBuffer::~NvrhiIndexBuffer() {}

} // namespace arch::gfx::nvrhi::buffer
