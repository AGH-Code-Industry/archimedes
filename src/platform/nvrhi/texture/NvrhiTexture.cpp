#include "NvrhiTexture.h"

#include "../NvrhiRenderer.h"
#include "../NvrhiUtils.h"
#include "../exception/NvrhiException.h"
#include "NvrhiTextureManager.h"

namespace arch::gfx::nvrhi::texture {

NvrhiTexture::NvrhiTexture(const CreateInfo& info, const WeakRef<NvrhiTextureManager>& textureManager):
	Texture(info),
	_textureManager(textureManager) {}

::nvrhi::SamplerHandle NvrhiTexture::getSampler() {
	if (!_sampler) {
		const auto textureManager = _textureManager.lock();
		_sampler = textureManager->_renderer.lock()->getDevice()->createSampler(_getSamplerDesc());
	}
	return _sampler;
}

void NvrhiTexture::setPixels(const void* pixels, u32 width, u32 height) {
	if (getWidth() != width || getHeight() != height) {
		throw exception::NvrhiException("Wrong texture ...");
	}

	const auto textureManager = _textureManager.lock();

	if (!_handle) {
		_handle = textureManager->_renderer.lock()->getDevice()->createTexture(_getDesc());
	}

	textureManager->_setTextureData(*this, pixels);
}

::nvrhi::TextureDesc NvrhiTexture::_getDesc() const {
	static int index = 0;
	return ::nvrhi::TextureDesc()
		.setWidth(getWidth())
		.setHeight(getHeight())
		.setDepth(getDepth())
		.setDimension(::nvrhi::TextureDimension::Texture2D)
		.setDebugName("Texture 2D: " + std::to_string(index++))
		.setFormat(NvrhiUtils::getFormat(getFormat()));
}

::nvrhi::SamplerDesc NvrhiTexture::_getSamplerDesc() const {
	auto desc = ::nvrhi::SamplerDesc();

	switch (getFilter()) {
		case gfx::texture::TextureFilterMode::nearestMipmapNearest:
		case gfx::texture::TextureFilterMode::nearest:				desc.setAllFilters(false); break;

		case gfx::texture::TextureFilterMode::linearMipmapLinear:
		case gfx::texture::TextureFilterMode::linear:			  desc.setAllFilters(true); break;

		case gfx::texture::TextureFilterMode::linearMipmapNearest: desc.setAllFilters(true).setMipFilter(false); break;

		case gfx::texture::TextureFilterMode::nearestMipmapLinear: desc.setAllFilters(false).setMipFilter(true); break;
	}

	switch (getWrap()) {
		case gfx::texture::TextureWrapMode::repeat: desc.setAllAddressModes(::nvrhi::SamplerAddressMode::Repeat); break;

		case gfx::texture::TextureWrapMode::mirroredRepeat:
			desc.setAllAddressModes(::nvrhi::SamplerAddressMode::MirroredRepeat);
			break;

		case gfx::texture::TextureWrapMode::clampToEdge:
			desc.setAllAddressModes(::nvrhi::SamplerAddressMode::ClampToEdge);
			break;

		case gfx::texture::TextureWrapMode::clampToBorder:
			desc.setAllAddressModes(::nvrhi::SamplerAddressMode::ClampToBorder);
			break;
	}

	return desc;
}

} // namespace arch::gfx::nvrhi::texture
