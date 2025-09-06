#pragma once
#include "nvrhi/nvrhi.h"
#include <archimedes/gfx/buffer/Buffer.h>

namespace arch::gfx::nvrhi::buffer {
class NvrhiBufferManager;

class NvrhiBuffer: public virtual gfx::buffer::Buffer {
public:
	NvrhiBuffer(gfx::buffer::BufferType type, const WeakRef<NvrhiBufferManager>& bufferManager);
	~NvrhiBuffer() override;

public:
	void setData(void* data, u64 size) override;

	::nvrhi::BufferHandle getNativeHandle() const { return _handle; }

	u64 getSize() const override { return _size; }

protected:
	virtual ::nvrhi::BufferDesc _getDesc(u64 size) const;

protected:
	WeakRef<NvrhiBufferManager> _bufferManager;

	::nvrhi::BufferHandle _handle;
	u64 _size = 0;
	u64 _capacity = 0;
};

} // namespace arch::gfx::nvrhi::buffer
