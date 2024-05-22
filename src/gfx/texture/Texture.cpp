#include "gfx/texture/Texture.h"

namespace arch::gfx::texture {

Texture::Texture(GraphicsFormat format, TextureWrapMode wrapMode, TextureFilterMode filterMode, bool isReadable):
	_isReadable(isReadable),
	_format(format),
	_wrapMode(wrapMode),
	_filterMode(filterMode) {}

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

bool Texture::isReadable() const {
	return _isReadable;
}

u32 Texture::getWidth() const {
	return getSize().x;
}

u32 Texture::getHeight() const {
	return getSize().y;
}

u32 Texture::getDepth() const {
	return getSize().z;
}

u32 Texture::getSize(u32 axis) const {
	return getSize()[axis];
}

} // namespace arch::gfx::texture
