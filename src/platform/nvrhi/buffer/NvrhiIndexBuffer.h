#pragma once
#include "NvrhiBuffer.h"
#include "gfx/buffer/IndexBuffer.h"

namespace arch::gfx::nvrhi::buffer {

class NvrhiIndexBuffer final: public gfx::buffer::IndexBuffer, public NvrhiBuffer {
public:
	NvrhiIndexBuffer();
	~NvrhiIndexBuffer() override;
};

} // namespace arch::gfx::nvrhi::buffer
