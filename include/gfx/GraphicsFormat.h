#pragma once
#include "ArchMath.h"

namespace arch::gfx {

enum class GraphicsFormat : u8 {
	none,

	r8,
	r8s,
	r8_norm,
	r8s_norm,

	r16,
	r16s,
	r16_norm,
	r16s_norm,
	r16f,

	rg8,
	rg8s,
	rg8_norm,
	rg8s_norm,

	bgra4_norm,
	b5g6r5_norm,
	b5g5r5a1_norm,
	rgba8,
	rgba8s,
	rgba8_norm,
	rgba8s_norm,
	bgra8_norm,
	srgba8_norm,
	sbgra8_norm,

	r10g10b10a2_norm,
	r11g11b10f,

	rg16,
	rg16s,
	rg16_norm,
	rg16s_norm,
	rg16f,

	r32,
	r32s,
	r32f,

	rgba16,
	rgba16s,
	rgba16f,
	rgba16_norm,
	rgba16s_norm,

	rg32,
	rg32s,
	rg32f,

	rgb32,
	rgb32s,
	rgb32f,

	rgba32,
	rgba32s,
	rgba32f,

	depth16,
	depth24Stencil8,
	depth32f,
	depth32fStencil8,

	_COUNT,
};

} // namespace arch::gfx
