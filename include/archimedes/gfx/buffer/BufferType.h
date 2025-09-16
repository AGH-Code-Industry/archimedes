#pragma once

namespace arch::gfx::buffer {

enum class BufferType {
	vertex,
	index,
	uniform,
	staging,
	blob,
};

} // namespace arch::gfx::buffer
