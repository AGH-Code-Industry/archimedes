#pragma once

namespace arch::gfx::texture {

enum class TextureWrapMode {
	repeat,
	mirroredRepeat,
	clampToEdge,
	clampToBorder,
};

} // namespace arch::gfx::texture
