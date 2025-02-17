#include "NvrhiBuffer.h"

#include "../NvrhiRenderer.h"
#include "NvrhiBufferManager.h"

namespace arch::gfx::nvrhi::buffer {

NvrhiBuffer::NvrhiBuffer(gfx::buffer::BufferType type, const WeakRef<NvrhiBufferManager>& bufferManager):
	Buffer(type),
	_bufferManager(bufferManager) {}

NvrhiBuffer::~NvrhiBuffer() {}

void NvrhiBuffer::setData(void* data, u64 size) {
	auto bufferManager = _bufferManager.lock();

	if (_size < size) {
		if (_handle) {
			_handle = nullptr;
		}

		_handle = bufferManager->_renderer.lock()->getDevice()->createBuffer(_getDesc(size));

		_size = size;
	}

	bufferManager->_setBufferData(*this, data, size);
}


::nvrhi::BufferDesc NvrhiBuffer::_getDesc(u64 size) const {
	static u64 index = 0;
	return ::nvrhi::BufferDesc().setByteSize(size).setDebugName("Buffer " + std::to_string(index++));
}

} // namespace arch::gfx::nvrhi::buffer
