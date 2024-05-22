#pragma once

#include "Ref.h"
#include "gfx/texture/TextureType.h"
#include "mmath.h"
#include <volk.h>

namespace arch::gfx::vulkan {
class VulkanContext;
} // namespace arch::gfx::vulkan

namespace arch::gfx::vulkan::texture {

class VulkanImage {
public:

	VulkanImage(
		const Ref<VulkanContext>& context,
		gfx::texture::TextureType type,
		uint3 size,
		u32 mipMapLevels,
		VkFormat format,
		VkImageTiling tiling,
		VkImageUsageFlags usage,
		VkMemoryPropertyFlags memoryFlags
	);
	~VulkanImage();

	void transitionImageLayout(VkImageLayout oldLayout, VkImageLayout newLayout);
	void createImageView(const Ref<VulkanContext>& context, u16 layerCount, i32 layerIndex, VkFormat format);

	void cleanup(const Ref<VulkanContext>& context);

public:
	VkImageView getImageView() const { return _imageView; }

	VkImage getImage() const { return _image; }

	VkDeviceMemory getMemory() const { return _memory; }

	VkMemoryRequirements getMemoryRequirements() const { return _memoryRequirements; }

	VkFormat getFormat() const { return _format; }

	VkExtent3D getExtent() const { return _extent; }

	u32 getMipMapLevels() const { return _mipMapLevels; }

private:
	gfx::texture::TextureType _type;

	VkImage _image = nullptr;
	VkDeviceMemory _memory = nullptr;
	VkImageView _imageView = nullptr;

	VkMemoryRequirements _memoryRequirements{};

	VkMemoryPropertyFlags _memoryFlags;

	VkFormat _format;
	VkExtent3D _extent;
	u32 _mipMapLevels;
};
} // namespace arch::gfx::vulkan::texture
