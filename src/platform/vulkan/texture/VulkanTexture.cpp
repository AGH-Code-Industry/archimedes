#include "VulkanTexture.h"

#include "../VulkanRenderer.h"
#include "../VulkanUtils.h"
#include "gfx/Renderer.h"

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
	Texture(format, wrapMode, filterMode, isReadable),
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

uint3 VulkanTexture::getSize() const {
	VkExtent3D extent = _image.getExtent();
	return { extent.width, extent.height, extent.depth };
}

VulkanImage& VulkanTexture::getImage() {
	return _image;
}

} // namespace arch::gfx::vulkan::texture
