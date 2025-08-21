#pragma once

namespace arch::gfx::texture {

enum class TextureFilterMode {
	nearest,
	linear,
	nearestMipmapNearest,
	linearMipmapNearest,
	nearestMipmapLinear,
	linearMipmapLinear,
};

} // namespace arch::gfx::texture