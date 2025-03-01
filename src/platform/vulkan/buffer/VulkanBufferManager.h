#pragma once

#include "../VulkanContext.h"
#include "gfx/Buffer.h"

namespace arch::gfx::vulkan::buffer {

class VulkanBufferManager final: public BufferManager {
public:
	explicit VulkanBufferManager(const Ref<VulkanContext>& context);
	~VulkanBufferManager() noexcept override = default;

private:
	Ref<VertexBuffer> _createVertexBufferImpl(void* data, u64 size, u64 vertexSize) override;
	Ref<IndexBuffer> _createIndexBufferImpl(std::span<u32> indices) override;
	Ref<Buffer> _createBufferImpl(BufferType type, void* data, u64 size) override;

private:
	Ref<VulkanContext> _context;
};

} // namespace arch::gfx::vulkan::buffer
