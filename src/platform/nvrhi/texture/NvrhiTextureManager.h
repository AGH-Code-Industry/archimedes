#pragma once
#include "gfx/texture/TextureManager.h"

namespace arch::gfx::nvrhi::texture {

class NvrhiTextureManager final: public gfx::texture::TextureManager {
public:
	NvrhiTextureManager() = default;
	~NvrhiTextureManager() override = default;

protected:
	Ref<gfx::texture::Texture> _createTexture2DImpl(
		u32 width,
		u32 height,
		GraphicsFormat format,
		void* data,
		gfx::texture::TextureWrapMode wrapMode,
		gfx::texture::TextureFilterMode filterMode,
		bool isReadable
	) override;
};

} // namespace arch::gfx::nvrhi::texture
