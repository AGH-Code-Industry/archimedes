#pragma once

#include "BufferType.h"
#include "Mmath.h"
#include "Ref.h"

namespace arch::gfx::buffer {

class Buffer {
protected:
	explicit Buffer(BufferType type): _type(type) {}

	virtual ~Buffer() = default;

public:
	static Ref<Buffer> create(BufferType type);
	static Ref<Buffer> create(size_t size, BufferType type);
	static Ref<Buffer> create(const void* data, size_t size, BufferType type);

public:
	virtual void setData(void* data, u64 size) = 0;

protected:
	BufferType _type;
};

} // namespace arch::gfx::buffer
