#include "VulkanTexture.h"

#include "../VulkanRenderer.h"
#include "../VulkanUtils.h"
#include <archimedes/gfx/Renderer.h>

namespace arch::gfx::vulkan::texture {

VulkanTexture::VulkanTexture(
	const Ref<VulkanContext>& context,
	uint3 size,
	TextureType type,
	GraphicsFormat format,
	TextureWrapMode wrapMode,
	TextureFilterMode filterMode,
	bool isReadable,
	VkImageUsageFlags usage
):
	Texture({ {}, format, wrapMode, filterMode }),
	_context(context),
	_image(
		context,
		type,
		size,
		1,
		VulkanUtils::getFormat(format),
		VK_IMAGE_TILING_LINEAR,
		usage,
		VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT
	) {}

VulkanTexture::~VulkanTexture() {
	_image.cleanup(_context.lock());
}

void VulkanTexture::setFilter(TextureFilterMode filterMode) {
	Texture::setFilter(filterMode);
}

void VulkanTexture::setWrap(TextureWrapMode wrapMode) {
	Texture::setWrap(wrapMode);
}

void VulkanTexture::setPixels(const void* pixels, u32 width, u32 height) {
	throw exceptions::VulkanException("Not implemented");
}

VulkanImage& VulkanTexture::getImage() {
	return _image;
}

} // namespace arch::gfx::vulkan::texture
