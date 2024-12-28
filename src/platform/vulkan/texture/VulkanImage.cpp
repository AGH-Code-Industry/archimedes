#include "VulkanImage.h"

#include "../VulkanContext.h"
#include "../VulkanUtils.h"

namespace arch::gfx::vulkan::texture {

VulkanImage::VulkanImage(
	const Ref<VulkanContext>& context,
	gfx::texture::TextureType type,
	uint3 size,
	u32 mipMapLevels,
	VkFormat format,
	VkImageTiling tiling,
	VkImageUsageFlags usage,
	VkMemoryPropertyFlags memoryFlags
):
	_type(type),
	_memoryFlags(memoryFlags),
	_format(format),
	_extent(size.x, size.y, size.z),
	_mipMapLevels(mipMapLevels) {
	VkImageCreateInfo imageInfo = {
		.sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
		.pNext = nullptr,
		.flags = 0,
		.imageType = VulkanUtils::getType(type),
		.format = _format,
		.extent = _extent,

		.mipLevels = 1,
		.arrayLayers = 1,

		// Anti-aliasing
		.samples = VK_SAMPLE_COUNT_1_BIT,

		// Tailing
		.tiling = tiling,
		.usage = usage,
	};
	VulkanUtils::vkAssert(
		vkCreateImage(context->getDevice(), &imageInfo, context->getAllocator(), &_image),
		"Failed to create image."
	);

	vkGetImageMemoryRequirements(context->getDevice(), _image, &_memoryRequirements);

	i32 memType = context->findMemoryType(_memoryRequirements.memoryTypeBits, _memoryFlags);
	if (memType < 0) {
		throw exceptions::VulkanException("Failed to find suitable memory type.");
	}

	// Allocate memory
	VkMemoryAllocateInfo allocInfo = {
		.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
		.allocationSize = _memoryRequirements.size,
		.memoryTypeIndex = (u32)memType,
	};
	VulkanUtils::vkAssert(
		vkAllocateMemory(context->getDevice(), &allocInfo, context->getAllocator(), &_memory),
		"Failed to allocate image memory."
	);

	// Bind the memory
	VulkanUtils::vkAssert(vkBindImageMemory(context->getDevice(), _image, _memory, 0), "Failed to bind image memory.");
}

VulkanImage::~VulkanImage() {
	if (_image || _imageView || _memory) {
		Logger::warn("Image was not destroyed properly. Make sure to call cleanup() before destroying the object.");
	}
}

void VulkanImage::transitionImageLayout(VkImageLayout oldLayout, VkImageLayout newLayout) {}

void VulkanImage::createImageView(const Ref<VulkanContext>& context, u16 layerCount, i32 layerIndex, VkFormat format) {
	VkImageAspectFlags aspectFlags = VK_IMAGE_ASPECT_COLOR_BIT;

	if (format == VK_FORMAT_D32_SFLOAT) {
		aspectFlags = VK_IMAGE_ASPECT_DEPTH_BIT;
	} else if (format == VK_FORMAT_D24_UNORM_S8_UINT || format == VK_FORMAT_D32_SFLOAT_S8_UINT) {
		aspectFlags = VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;
	}

	VkImageViewCreateInfo viewInfo = {
		.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
		.image = _image,
		.viewType = VulkanUtils::getImageViewType(_type),
		.format = format,
		.components = {
			.r = VK_COMPONENT_SWIZZLE_IDENTITY,
			.g = VK_COMPONENT_SWIZZLE_IDENTITY,
			.b = VK_COMPONENT_SWIZZLE_IDENTITY,
			.a = VK_COMPONENT_SWIZZLE_IDENTITY,
		},
		.subresourceRange = {
			.aspectMask = aspectFlags,
			.baseMipLevel = 0,
			.levelCount = _mipMapLevels,
			.baseArrayLayer = 0,
			.layerCount = VK_REMAINING_ARRAY_LAYERS,
		}
	};

	VulkanUtils::vkAssert(
		vkCreateImageView(context->getDevice(), &viewInfo, context->getAllocator(), &_imageView),
		"Failed to create image view."
	);
}

void VulkanImage::cleanup(const Ref<VulkanContext>& context) {
	if (_imageView) {
		vkDestroyImageView(context->getDevice(), _imageView, context->getAllocator());
		_imageView = nullptr;
	}

	if (_memory) {
		vkFreeMemory(context->getDevice(), _memory, context->getAllocator());
		_memory = nullptr;
	}

	if (_image) {
		vkDestroyImage(context->getDevice(), _image, context->getAllocator());
		_image = nullptr;
	}

	_memoryRequirements = {};
}

} // namespace arch::gfx::vulkan::texture
