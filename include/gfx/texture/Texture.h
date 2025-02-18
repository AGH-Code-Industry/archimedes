#pragma once

#include "TextureFilterMode.h"
#include "TextureWrapMode.h"
#include "gfx/GraphicsFormat.h"

namespace arch::gfx::texture {

class Texture {
public:
	struct CreateInfo {
		uint3 size;

		GraphicsFormat format;
		TextureWrapMode wrapMode;
		TextureFilterMode filterMode;
	};
protected:
	Texture(const CreateInfo& info);
	virtual ~Texture() = default;

public:
	TextureFilterMode getFilter() const;
	virtual void setFilter(TextureFilterMode filterMode);

	TextureWrapMode getWrap() const;
	virtual void setWrap(TextureWrapMode wrapMode);

	GraphicsFormat getFormat() const;

	bool isReadable() const;

	u32 getWidth() const;
	u32 getHeight() const;
	u32 getDepth() const;
	u32 getSize(u32 axis) const;
	uint3 getSize() const;

public:

	// virtual void setPixels(std::span<Color> pixels) = 0;
	virtual void setPixels(Color* pixels, u32 width, u32 height) = 0;

	// virtual void setPixel(u32 x, u32 y, Color color) = 0;

	// virtual void readPixels() = 0;

private:
	uint3 _size;

	GraphicsFormat _format;
	TextureWrapMode _wrapMode;
	TextureFilterMode _filterMode;
};

} // namespace arch::gfx::texture
