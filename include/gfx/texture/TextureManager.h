#pragma once

#include <span>

#include "Ref.h"
#include "Texture2D.h"

namespace arch::gfx::texture {

class TextureManager {
public:
	Ref<Texture2D> createTexture2D();
	Ref<Texture2D> createTexture2D(std::span<Color> pixels, u32 width, u32 height);
};

} // namespace arch::gfx::texture
