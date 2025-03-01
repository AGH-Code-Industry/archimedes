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

	if (_capacity < size) {
		if (_handle) {
			_handle = nullptr;
		}

		_handle = bufferManager->_renderer.lock()->getDevice()->createBuffer(_getDesc(size));

		_capacity = size;
	}
	_size = size;

	bufferManager->_setBufferData(*this, data, size);
}

::nvrhi::BufferDesc NvrhiBuffer::_getDesc(u64 size) const {
	static u64 index = 0;
	auto desc = ::nvrhi::BufferDesc().setByteSize(size).setDebugName("Buffer " + std::to_string(index++));

	switch (_type) {
		case gfx::buffer::BufferType::uniform:
			desc.setIsConstantBuffer(true)
				.setInitialState(::nvrhi::ResourceStates::ShaderResource)
				.setKeepInitialState(true); //.setIsVolatile(true).setMaxVersions(16);
			break;

		case gfx::buffer::BufferType::vertex: desc.setIsVertexBuffer(true); break;
		case gfx::buffer::BufferType::index:  desc.setIsIndexBuffer(true); break;

		case gfx::buffer::BufferType::blob: desc.setCanHaveRawViews(true); break;

		default: throw InvalidEnumException("Invalid BufferType");
	}

	return desc;
}

} // namespace arch::gfx::nvrhi::buffer
