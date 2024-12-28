#pragma once
#include <source_location>
#include <string>

#include "gfx/GraphicsFormat.h"
#include "gfx/texture/TextureType.h"
#include <volk.h>

namespace arch::gfx::vulkan {

class VulkanUtils {
public:
	static void vkAssert(
		VkResult result,
		const std::string& message,
		const std::source_location& location = std::source_location::current()
	);

	static VkFormat getFormat(GraphicsFormat format);
	static GraphicsFormat getFormat(VkFormat format);

	static VkImageType getType(gfx::texture::TextureType type);
	static VkImageViewType getImageViewType(gfx::texture::TextureType type);
};

} // namespace arch::gfx::vulkan
