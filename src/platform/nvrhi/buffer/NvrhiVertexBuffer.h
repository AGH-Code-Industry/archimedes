#pragma once
#include "NvrhiBuffer.h"
#include "gfx/buffer/VertexBuffer.h"
#include "nvrhi/nvrhi.h"

namespace arch::gfx::nvrhi::buffer {

class NvrhiVertexBuffer final: public gfx::buffer::VertexBuffer, public NvrhiBuffer {
public:
	NvrhiVertexBuffer();
	~NvrhiVertexBuffer() override;
};

} // namespace arch::gfx::nvrhi::buffer
