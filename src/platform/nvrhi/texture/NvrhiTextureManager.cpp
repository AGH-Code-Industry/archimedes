#include "NvrhiTextureManager.h"

#include "../NvrhiRenderer.h"
#include "NvrhiTexture.h"

namespace arch::gfx::nvrhi::texture {

NvrhiTextureManager::NvrhiTextureManager(const WeakRef<NvrhiRenderer>& renderer):
	_renderer(renderer) {
	_stageCommandBuffer = renderer.lock()->getDevice()->createCommandList();
}

Ref<gfx::texture::Texture> NvrhiTextureManager::_createTexture2DImpl(
	u32 width,
	u32 height,
	GraphicsFormat format,
	void* data,
	gfx::texture::TextureWrapMode wrapMode,
	gfx::texture::TextureFilterMode filterMode,
	bool isReadable
) {
	auto texture = createRef<NvrhiTexture>(
		gfx::texture::Texture::CreateInfo{
			{ width, height, 1 },
			format,
			wrapMode,
			filterMode,
	}, weak_from_this());

	if (data)
		texture->setPixels((Color*)data, width, height);

	return texture;
}

void NvrhiTextureManager::_setTextureData(const NvrhiTexture& texture, const void* data) const {
	_stageCommandBuffer->open();
	_stageCommandBuffer->writeTexture(texture.getNativeHandle(), 0, 0, data, texture.getWidth() * sizeof(Color));
	_stageCommandBuffer->close();

	_renderer.lock()->getDevice()->executeCommandList(_stageCommandBuffer);
}

} // namespace arch::gfx::nvrhi::texture
