#pragma once

#include "Buffer.h"

namespace arch::gfx::buffer {

class IndexBuffer: public virtual Buffer {
public:
	IndexBuffer(): Buffer(BufferType::index) {}
};

} // namespace arch::gfx::buffer
