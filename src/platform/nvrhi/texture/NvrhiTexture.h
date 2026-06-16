#pragma once
#include "nvrhi/nvrhi.h"
#include <archimedes/Ref.h>
#include <archimedes/gfx/texture/Texture.h>

namespace arch::gfx::nvrhi::texture {
class NvrhiTextureManager;

class NvrhiTexture: public gfx::texture::Texture {
public:
	NvrhiTexture(const CreateInfo& info, const WeakRef<NvrhiTextureManager>& textureManager);
	~NvrhiTexture() override = default;

	::nvrhi::TextureHandle getNativeHandle() const { return _handle; }

	::nvrhi::SamplerHandle getSampler();

public:
	void setPixels(const void* pixels, u32 width, u32 height) override;

protected:
	virtual ::nvrhi::TextureDesc _getDesc() const;
	virtual ::nvrhi::SamplerDesc _getSamplerDesc() const;

protected:
	WeakRef<NvrhiTextureManager> _textureManager;

	::nvrhi::TextureHandle _handle;
	::nvrhi::SamplerHandle _sampler;
};

} // namespace arch::gfx::nvrhi::texture
