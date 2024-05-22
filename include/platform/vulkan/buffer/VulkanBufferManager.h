#pragma once

#include "gfx/Buffer.h"
#include "platform/vulkan/VulkanContext.h"

namespace arch::gfx::vulkan::buffer {

class VulkanBufferManager final: public BufferManager {
public:
	explicit VulkanBufferManager(const Ref<VulkanContext>& context);
	~VulkanBufferManager() noexcept override = default;

private:
	Ref<VertexBuffer> _createVertexBufferImpl(void* data, u32 size) override;
	Ref<IndexBuffer> _createIndexBufferImpl(std::span<u32> indices) override;
	Ref<Buffer> _createBufferImpl(void* data, u32 size) override;

private:
	Ref<VulkanContext> _context;
};

} // namespace arch::gfx::vulkan::buffer
