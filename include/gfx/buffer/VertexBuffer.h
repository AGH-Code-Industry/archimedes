#pragma once
#include "Buffer.h"

namespace arch::gfx::buffer {

class VertexBuffer: public Buffer {
public:
	VertexBuffer(): Buffer(BufferType::vertex) {}
};

} // namespace arch::gfx::buffer
