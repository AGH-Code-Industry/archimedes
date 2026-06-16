#include <archimedes/gfx/texture/TextureManager.h>

namespace arch::gfx::texture {

Ref<Texture> TextureManager::createTexture2D(
	u32 width,
	u32 height,
	Color* pixels,
	GraphicsFormat format,
	TextureWrapMode wrapMode,
	TextureFilterMode filterMode,
	bool isReadable
) {
	return _createTexture2DImpl(width, height, format, pixels, wrapMode, filterMode, isReadable);
}

Ref<Texture> TextureManager::createTexture2D(
	u32 width,
	u32 height,
	GraphicsFormat format,
	void* pixels,
	TextureWrapMode wrapMode,
	TextureFilterMode filterMode,
	bool isReadable
) {
	return _createTexture2DImpl(width, height, format, pixels, wrapMode, filterMode, isReadable);
}

} // namespace arch::gfx::texture
