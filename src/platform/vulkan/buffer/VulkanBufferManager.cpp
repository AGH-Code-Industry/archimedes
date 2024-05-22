#include "platform/vulkan/buffer/VulkanBufferManager.h"

namespace arch::gfx::vulkan::buffer {

VulkanBufferManager::VulkanBufferManager(const Ref<VulkanContext>& context): _context(context) {}

Ref<VertexBuffer> VulkanBufferManager::_createVertexBufferImpl(void* data, u32 size) {
	return nullptr;
}

Ref<IndexBuffer> VulkanBufferManager::_createIndexBufferImpl(std::span<u32> indices) {
	return nullptr;
}

Ref<Buffer> VulkanBufferManager::_createBufferImpl(void* data, u32 size) {
	return nullptr;
}

} // namespace arch::gfx::vulkan::buffer
