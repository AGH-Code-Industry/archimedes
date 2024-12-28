#pragma once
#include "gfx/buffer/Buffer.h"
#include "nvrhi/nvrhi.h"

namespace arch::gfx::nvrhi::buffer {

class NvrhiBuffer: public virtual gfx::buffer::Buffer {
public:
	NvrhiBuffer(gfx::buffer::BufferType type);
	~NvrhiBuffer() override;

public:
	void setData(void* data, u64 size) const override;

private:
	::nvrhi::BufferHandle _buffer;
};

} // namespace arch::gfx::nvrhi::buffer
