#include "NvrhiBuffer.h"

namespace arch::gfx::nvrhi::buffer {

NvrhiBuffer::NvrhiBuffer(gfx::buffer::BufferType type): Buffer(type) {}

NvrhiBuffer::~NvrhiBuffer() {}

void NvrhiBuffer::setData(void* data, u64 size) const {}

} // namespace arch::gfx::nvrhi::buffer
