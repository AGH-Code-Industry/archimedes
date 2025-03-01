#pragma once
#include "NvrhiTexture.h"
#include "gfx/texture/TextureManager.h"
#include "nvrhi/nvrhi.h"

namespace arch::gfx::nvrhi {
class NvrhiRenderer;
}

namespace arch::gfx::nvrhi::texture {

class NvrhiTextureManager final: public gfx::texture::TextureManager, public std::enable_shared_from_this<NvrhiTextureManager> {
public:
	NvrhiTextureManager(const WeakRef<NvrhiRenderer>& renderer);
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

private:
	friend class NvrhiTexture;

	void _setTextureData(const NvrhiTexture& texture, const void* data) const;
private:
	WeakRef<NvrhiRenderer> _renderer;
	::nvrhi::CommandListHandle _stageCommandBuffer;
};

} // namespace arch::gfx::nvrhi::texture
