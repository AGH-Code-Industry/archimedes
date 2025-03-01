#include "VulkanBufferManager.h"

namespace arch::gfx::vulkan::buffer {

VulkanBufferManager::VulkanBufferManager(const Ref<VulkanContext>& context): _context(context) {}

Ref<VertexBuffer> VulkanBufferManager::_createVertexBufferImpl(void* data, u64 size, u64 vertexSize) {
	return nullptr;
}

Ref<IndexBuffer> VulkanBufferManager::_createIndexBufferImpl(std::span<u32> indices) {
	return nullptr;
}

Ref<Buffer> VulkanBufferManager::_createBufferImpl(BufferType type, void* data, u64 size) {
	return nullptr;
}

} // namespace arch::gfx::vulkan::buffer
