#include "gfx/texture/Texture.h"

namespace arch::gfx::texture {

Texture::Texture(const CreateInfo& info):
	_size(info.size),
	_format(info.format),
	_wrapMode(info.wrapMode),
	_filterMode(info.filterMode) {}

TextureFilterMode Texture::getFilter() const {
	return _filterMode;
}

void Texture::setFilter(TextureFilterMode filterMode) {
	_filterMode = filterMode;
}

TextureWrapMode Texture::getWrap() const {
	return _wrapMode;
}

void Texture::setWrap(TextureWrapMode wrapMode) {
	_wrapMode = wrapMode;
}

GraphicsFormat Texture::getFormat() const {
	return _format;
}

u32 Texture::getWidth() const {
	return _size.x;
}

u32 Texture::getHeight() const {
	return _size.y;
}

u32 Texture::getDepth() const {
	return _size.z;
}

u32 Texture::getSize(u32 axis) const {
	return _size[axis];
}

uint3 Texture::getSize() const {
	return _size;
}

} // namespace arch::gfx::texture
