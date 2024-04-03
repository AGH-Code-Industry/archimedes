#pragma once

#include "Buffer.h"

namespace arch::gfx::buffer {

class IndexBuffer: public Buffer {
public:
	IndexBuffer(): Buffer(BufferType::index) {}
};

} // namespace arch::gfx::buffer
