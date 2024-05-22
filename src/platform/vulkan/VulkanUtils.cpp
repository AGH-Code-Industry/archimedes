#include "platform/vulkan/VulkanUtils.h"

#include "Logger.h"
#include "platform/vulkan/exceptions/VulkanException.h"

namespace arch::gfx::vulkan {

void VulkanUtils::vkAssert(VkResult result, const std::string& message, const std::source_location& location) {
	if (result != VK_SUCCESS) {
		throw exceptions::VulkanException(message, location);
	}
}

VkFormat VulkanUtils::getFormat(GraphicsFormat format) {
	switch (format) {
		case GraphicsFormat::rgba8:			   return VK_FORMAT_R8G8B8A8_UINT;
		case GraphicsFormat::rgb8:			   return VK_FORMAT_R8G8B8_UINT;
		case GraphicsFormat::rg8:			   return VK_FORMAT_R8G8_UINT;
		case GraphicsFormat::r8:			   return VK_FORMAT_R8_UINT;
		case GraphicsFormat::rgba16f:		   return VK_FORMAT_R16G16B16A16_SFLOAT;
		case GraphicsFormat::rgb16f:		   return VK_FORMAT_R16G16B16_SFLOAT;
		case GraphicsFormat::rg16f:			   return VK_FORMAT_R16G16_SFLOAT;
		case GraphicsFormat::r16f:			   return VK_FORMAT_R16_SFLOAT;
		case GraphicsFormat::rgba32f:		   return VK_FORMAT_R32G32B32A32_SFLOAT;
		case GraphicsFormat::rgb32f:		   return VK_FORMAT_R32G32B32_SFLOAT;
		case GraphicsFormat::rg32f:			   return VK_FORMAT_R32G32_SFLOAT;
		case GraphicsFormat::r32f:			   return VK_FORMAT_R32_SFLOAT;
		case GraphicsFormat::depth24Stencil8:  return VK_FORMAT_D24_UNORM_S8_UINT;
		case GraphicsFormat::depth32f:		   return VK_FORMAT_D32_SFLOAT;
		case GraphicsFormat::depth32fStencil8: return VK_FORMAT_D32_SFLOAT_S8_UINT;

		default:
		case GraphicsFormat::none: return VK_FORMAT_UNDEFINED;
	}
}

GraphicsFormat VulkanUtils::getFormat(VkFormat format) {
	switch (format) {
		case VK_FORMAT_R8G8B8A8_UINT:		return GraphicsFormat::rgba8;
		case VK_FORMAT_R8G8B8_UINT:			return GraphicsFormat::rgb8;
		case VK_FORMAT_R8G8_UINT:			return GraphicsFormat::rg8;
		case VK_FORMAT_R8_UINT:				return GraphicsFormat::r8;
		case VK_FORMAT_R16G16B16A16_SFLOAT: return GraphicsFormat::rgba16f;
		case VK_FORMAT_R16G16B16_SFLOAT:	return GraphicsFormat::rgb16f;
		case VK_FORMAT_R16G16_SFLOAT:		return GraphicsFormat::rg16f;
		case VK_FORMAT_R16_SFLOAT:			return GraphicsFormat::r16f;
		case VK_FORMAT_R32G32B32A32_SFLOAT: return GraphicsFormat::rgba32f;
		case VK_FORMAT_R32G32B32_SFLOAT:	return GraphicsFormat::rgb32f;
		case VK_FORMAT_R32G32_SFLOAT:		return GraphicsFormat::rg32f;
		case VK_FORMAT_R32_SFLOAT:			return GraphicsFormat::r32f;
		case VK_FORMAT_D24_UNORM_S8_UINT:	return GraphicsFormat::depth24Stencil8;
		case VK_FORMAT_D32_SFLOAT:			return GraphicsFormat::depth32f;
		case VK_FORMAT_D32_SFLOAT_S8_UINT:	return GraphicsFormat::depth32fStencil8;

		default:
		case VK_FORMAT_UNDEFINED: return GraphicsFormat::none;
	}
}

VkImageType VulkanUtils::getType(texture::TextureType type) {
	switch (type) {
		case texture::TextureType::texture2D:
			return VK_IMAGE_TYPE_2D;

			// case texture::TextureType::texture3D: return VK_IMAGE_TYPE_3D;

		default: return VK_IMAGE_TYPE_MAX_ENUM;
	}
}

VkImageViewType VulkanUtils::getImageViewType(texture::TextureType type) {
	switch (type) {
		case texture::TextureType::texture2D:
			return VK_IMAGE_VIEW_TYPE_2D;

			// case texture::TextureType::texture3D: return VK_IMAGE_VIEW_TYPE_3D;

		default: return VK_IMAGE_VIEW_TYPE_MAX_ENUM;
	}
}

} // namespace arch::gfx::vulkan
