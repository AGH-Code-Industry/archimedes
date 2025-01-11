#pragma once
#include "NvrhiBuffer.h"
#include "gfx/buffer/VertexBuffer.h"
#include "nvrhi/nvrhi.h"

namespace arch::gfx::nvrhi::buffer {

class NvrhiVertexBuffer final: public gfx::buffer::VertexBuffer, public NvrhiBuffer {
public:
	NvrhiVertexBuffer(const WeakRef<NvrhiBufferManager>& bufferManager);
	~NvrhiVertexBuffer() override;

protected:
	::nvrhi::BufferDesc _getDesc(u64 size) const override;
};

} // namespace arch::gfx::nvrhi::buffer
