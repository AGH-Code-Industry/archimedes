#pragma once

#include "Ref.h"
#include "Texture.h"

namespace arch::gfx::texture {

class Texture2D: public Texture {
public:
	void setPixels(std::span<Color> pixels);
	virtual void setPixels(std::span<Color> pixels, u32 width, u32 height) = 0;

	virtual void setPixel(u32 x, u32 y, Color color) = 0;

	virtual void readPixel() = 0;
};

} // namespace arch::gfx::texture
