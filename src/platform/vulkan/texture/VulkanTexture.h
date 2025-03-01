#pragma once

#include "VulkanImage.h"
#include "gfx/Texture.h"
#include <volk.h>

namespace arch::gfx::vulkan {
class VulkanContext;
}

namespace arch::gfx::vulkan::texture {

class VulkanTexture final: public Texture {
public:
	VulkanTexture(
		const Ref<VulkanContext>& context,
		uint3 size,
		TextureType type,
		GraphicsFormat format,
		TextureWrapMode wrapMode,
		TextureFilterMode filterMode,
		bool isReadable,
		VkImageUsageFlags usage = VK_IMAGE_USAGE_SAMPLED_BIT
	);
	~VulkanTexture() override;

	void setFilter(TextureFilterMode filterMode) override;
	void setWrap(TextureWrapMode wrapMode) override;

	void setPixels(const void* pixels, u32 width, u32 height) override;

public:
	VulkanImage& getImage();

private:
	WeakRef<VulkanContext> _context;
	VulkanImage _image;

	friend class VulkanTextureManager;
};

} // namespace arch::gfx::vulkan::texture
