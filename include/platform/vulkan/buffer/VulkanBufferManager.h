#pragma once

#include "gfx/buffer.h"

namespace arch::gfx::vulkan::buffer {

class VulkanBufferManager: public BufferManager {
protected:
	Ref<VertexBuffer> _createVertexBufferImpl(void* data, u32 size) override;
	Ref<IndexBuffer> _createIndexBufferImpl(std::span<u32> indices) override;
	Ref<Buffer> _createBufferImpl(void* data, u32 size) override;
};

} // namespace arch::gfx::vulkan::buffer
