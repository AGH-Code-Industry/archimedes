#pragma once
#include "gfx/Texture.h"
#include "platform/vulkan/VulkanContext.h"

namespace arch::gfx::vulkan::texture {

class VulkanTextureManager final: public TextureManager {
public:
	explicit VulkanTextureManager(const Ref<VulkanContext>& renderer);
	~VulkanTextureManager() noexcept override = default;

protected:
	Ref<Texture> _createTexture2DImpl(
		u32 width,
		u32 height,
		GraphicsFormat format,
		void* data,
		TextureWrapMode wrapMode,
		TextureFilterMode filterMode,
		bool isReadable
	) override;

private:
	Ref<VulkanContext> _context;
	friend class VulkanRenderer;
};

} // namespace arch::gfx::vulkan::texture
