#pragma once

namespace arch::gfx {

enum class GraphicsFormat {
	none,

	rgba8,
	rgb8,
	rg8,
	r8,

	rgba16f,
	rgb16f,
	rg16f,
	r16f,

	rgba32f,
	rgb32f,
	rg32f,
	r32f,

	depth24Stencil8,
	depth32f,
	depth32fStencil8,
};

} // namespace arch::gfx
