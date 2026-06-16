#pragma once

#include <filesystem>

#include "Texture.h"
#include <archimedes/Ref.h>

namespace arch::gfx::texture {

class TextureManager {
protected:
	TextureManager() = default;

public:
	virtual ~TextureManager() = default;

	Ref<Texture> createTexture2D(
		u32 width,
		u32 height,
		Color* pixels = nullptr,
		GraphicsFormat format = GraphicsFormat::rgba32f,
		TextureWrapMode wrapMode = TextureWrapMode::repeat,
		TextureFilterMode filterMode = TextureFilterMode::linear,
		bool isReadable = false
	);
	Ref<Texture> createTexture2D(
		u32 width,
		u32 height,
		GraphicsFormat format,
		void* pixels,
		TextureWrapMode wrapMode = TextureWrapMode::repeat,
		TextureFilterMode filterMode = TextureFilterMode::linear,
		bool isReadable = false
	);

protected:
	virtual Ref<Texture> _createTexture2DImpl(
		u32 width,
		u32 height,
		GraphicsFormat format,
		void* data,
		TextureWrapMode wrapMode,
		TextureFilterMode filterMode,
		bool isReadable
	) = 0;
};

} // namespace arch::gfx::texture
