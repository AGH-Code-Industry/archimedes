#include "platform/nvrhi/buffer/NvrhiBufferManager.h"

#include "platform/nvrhi/buffer/NvrhiIndexBuffer.h"
#include "platform/nvrhi/buffer/NvrhiVertexBuffer.h"

namespace arch::gfx::nvrhi::buffer {

Ref<gfx::buffer::VertexBuffer> NvrhiBufferManager::_createVertexBufferImpl(void* data, u32 size) {
	return createRef<NvrhiVertexBuffer>();
}

Ref<gfx::buffer::IndexBuffer> NvrhiBufferManager::_createIndexBufferImpl(std::span<u32> indices) {
	return createRef<NvrhiIndexBuffer>();
}

Ref<gfx::buffer::Buffer> NvrhiBufferManager::_createBufferImpl(void* data, u32 size) {
	return createRef<NvrhiBuffer>(gfx::buffer::BufferType::blob);
}

} // namespace arch::gfx::nvrhi::buffer
