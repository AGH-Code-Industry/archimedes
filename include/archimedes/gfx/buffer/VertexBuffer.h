#pragma once
#include "Buffer.h"

namespace arch::gfx::buffer {

class VertexBuffer: public virtual Buffer {
public:
	VertexBuffer(u64 vertexSize): Buffer(BufferType::vertex), _vertexSize(vertexSize) {}

	~VertexBuffer() override = default;

public:
	u64 getVertexSize() const { return _vertexSize; }

	u64 getVertexCount() const { return getSize() / _vertexSize; }

private:
	u64 _vertexSize;
};

} // namespace arch::gfx::buffer
