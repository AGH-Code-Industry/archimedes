#pragma once
#include "NvrhiBuffer.h"
#include "nvrhi/nvrhi.h"
#include <archimedes/gfx/buffer/VertexBuffer.h>

namespace arch::gfx::nvrhi::buffer {

class NvrhiVertexBuffer final: public gfx::buffer::VertexBuffer, public NvrhiBuffer {
public:
	NvrhiVertexBuffer(u64 vertexSize, const WeakRef<NvrhiBufferManager>& bufferManager);
	~NvrhiVertexBuffer() override;

protected:
	::nvrhi::BufferDesc _getDesc(u64 size) const override;
};

} // namespace arch::gfx::nvrhi::buffer
