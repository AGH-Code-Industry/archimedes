#pragma once

#include "Buffer.h"

namespace arch::gfx::buffer {

class IndexBuffer: public virtual Buffer {
public:
	IndexBuffer(): Buffer(BufferType::index) {}

public:
	u64 getIndexCount() const { return getSize() / sizeof(u32); }
};

} // namespace arch::gfx::buffer
