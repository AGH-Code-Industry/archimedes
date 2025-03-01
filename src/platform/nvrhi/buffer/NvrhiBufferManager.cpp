#include "NvrhiBufferManager.h"

#include "../NvrhiRenderer.h"
#include "NvrhiIndexBuffer.h"
#include "NvrhiVertexBuffer.h"

namespace arch::gfx::nvrhi::buffer {

NvrhiBufferManager::NvrhiBufferManager(const WeakRef<NvrhiRenderer>& renderer): _renderer(renderer) {
	_stageCommandBuffer = renderer.lock()->getDevice()->createCommandList();
}

void NvrhiBufferManager::_setBufferData(const NvrhiBuffer& buffer, const void* data, u64 size) const {
	_stageCommandBuffer->open();
	_stageCommandBuffer->writeBuffer(buffer.getNativeHandle(), data, size);
	_stageCommandBuffer->close();

	_renderer.lock()->getDevice()->executeCommandList(_stageCommandBuffer);
}

Ref<gfx::buffer::VertexBuffer> NvrhiBufferManager::_createVertexBufferImpl(void* data, u64 size, u64 vertexSize) {
	auto buffer = createRef<NvrhiVertexBuffer>(vertexSize, weak_from_this());
	if (data && size > 0) {
		buffer->setData(data, size);
	}
	return buffer;
}

Ref<gfx::buffer::IndexBuffer> NvrhiBufferManager::_createIndexBufferImpl(std::span<u32> indices) {
	auto buffer = createRef<NvrhiIndexBuffer>(weak_from_this());
	if (!indices.empty()) {
		buffer->setData(indices.data(), indices.size() * sizeof(u32));
	}
	return buffer;
}

Ref<gfx::buffer::Buffer> NvrhiBufferManager::_createBufferImpl(gfx::buffer::BufferType type, void* data, u64 size) {
	auto buffer = createRef<NvrhiBuffer>(type, weak_from_this());
	if (data && size > 0) {
		buffer->setData(data, size);
	}
	return buffer;
}

} // namespace arch::gfx::nvrhi::buffer
