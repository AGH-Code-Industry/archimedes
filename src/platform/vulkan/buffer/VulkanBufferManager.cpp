#include "platform/vulkan/buffer/VulkanBufferManager.h"

namespace arch::gfx::vulkan::buffer {

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
