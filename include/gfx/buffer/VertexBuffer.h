#pragma once
#include "Buffer.h"

namespace arch::gfx::buffer {

class VertexBuffer: public virtual Buffer {
public:
	VertexBuffer(): Buffer(BufferType::vertex) {}

	~VertexBuffer() override = default;
};

} // namespace arch::gfx::buffer
