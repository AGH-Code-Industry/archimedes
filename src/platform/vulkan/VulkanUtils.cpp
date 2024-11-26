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
		default:
		case GraphicsFormat::none: return VK_FORMAT_UNDEFINED;

		case GraphicsFormat::r8:			   return VK_FORMAT_R8_UINT;
		case GraphicsFormat::r8s:			   return VK_FORMAT_R8_SINT;
		case GraphicsFormat::r8_norm:		   return VK_FORMAT_R8_UNORM;
		case GraphicsFormat::r8s_norm:		   return VK_FORMAT_R8_SNORM;
		case GraphicsFormat::r16:			   return VK_FORMAT_R16_UINT;
		case GraphicsFormat::r16s:			   return VK_FORMAT_R16_SINT;
		case GraphicsFormat::r16_norm:		   return VK_FORMAT_R16_UNORM;
		case GraphicsFormat::r16s_norm:		   return VK_FORMAT_R16_SNORM;
		case GraphicsFormat::r16f:			   return VK_FORMAT_R16_SFLOAT;
		case GraphicsFormat::rg8:			   return VK_FORMAT_R8G8_UINT;
		case GraphicsFormat::rg8s:			   return VK_FORMAT_R8G8_SINT;
		case GraphicsFormat::rg8_norm:		   return VK_FORMAT_R8G8_UNORM;
		case GraphicsFormat::rg8s_norm:		   return VK_FORMAT_R8G8_SNORM;
		case GraphicsFormat::bgra4_norm:	   return VK_FORMAT_B4G4R4A4_UNORM_PACK16;
		case GraphicsFormat::b5g6r5_norm:	   return VK_FORMAT_B5G6R5_UNORM_PACK16;
		case GraphicsFormat::b5g5r5a1_norm:	   return VK_FORMAT_B5G5R5A1_UNORM_PACK16;
		case GraphicsFormat::rgba8:			   return VK_FORMAT_R8G8B8A8_UINT;
		case GraphicsFormat::rgba8s:		   return VK_FORMAT_R8G8B8A8_SINT;
		case GraphicsFormat::rgba8_norm:	   return VK_FORMAT_R8G8B8A8_UNORM;
		case GraphicsFormat::rgba8s_norm:	   return VK_FORMAT_R8G8B8A8_SNORM;
		case GraphicsFormat::bgra8_norm:	   return VK_FORMAT_B8G8R8A8_UNORM;
		case GraphicsFormat::srgba8_norm:	   return VK_FORMAT_R8G8B8A8_SRGB;
		case GraphicsFormat::sbgra8_norm:	   return VK_FORMAT_B8G8R8A8_SRGB;
		case GraphicsFormat::r10g10b10a2_norm: return VK_FORMAT_A2B10G10R10_UNORM_PACK32;
		case GraphicsFormat::r11g11b10f:	   return VK_FORMAT_B10G11R11_UFLOAT_PACK32;
		case GraphicsFormat::rg16:			   return VK_FORMAT_R16G16_UINT;
		case GraphicsFormat::rg16s:			   return VK_FORMAT_R16G16_SINT;
		case GraphicsFormat::rg16_norm:		   return VK_FORMAT_R16G16_UNORM;
		case GraphicsFormat::rg16s_norm:	   return VK_FORMAT_R16G16_SNORM;
		case GraphicsFormat::rg16f:			   return VK_FORMAT_R16G16_SFLOAT;
		case GraphicsFormat::r32:			   return VK_FORMAT_R32_UINT;
		case GraphicsFormat::r32s:			   return VK_FORMAT_R32_SINT;
		case GraphicsFormat::r32f:			   return VK_FORMAT_R32_SFLOAT;
		case GraphicsFormat::rgba16:		   return VK_FORMAT_R16G16B16A16_UINT;
		case GraphicsFormat::rgba16s:		   return VK_FORMAT_R16G16B16A16_SINT;
		case GraphicsFormat::rgba16f:		   return VK_FORMAT_R16G16B16A16_SFLOAT;
		case GraphicsFormat::rgba16_norm:	   return VK_FORMAT_R16G16B16A16_UNORM;
		case GraphicsFormat::rgba16s_norm:	   return VK_FORMAT_R16G16B16A16_SNORM;
		case GraphicsFormat::rg32:			   return VK_FORMAT_R32G32_UINT;
		case GraphicsFormat::rg32s:			   return VK_FORMAT_R32G32_SINT;
		case GraphicsFormat::rg32f:			   return VK_FORMAT_R32G32_SFLOAT;
		case GraphicsFormat::rgb32:			   return VK_FORMAT_R32G32B32_UINT;
		case GraphicsFormat::rgb32s:		   return VK_FORMAT_R32G32B32_SINT;
		case GraphicsFormat::rgb32f:		   return VK_FORMAT_R32G32B32_SFLOAT;
		case GraphicsFormat::rgba32:		   return VK_FORMAT_R32G32B32A32_UINT;
		case GraphicsFormat::rgba32s:		   return VK_FORMAT_R32G32B32A32_SINT;
		case GraphicsFormat::rgba32f:		   return VK_FORMAT_R32G32B32A32_SFLOAT;
		case GraphicsFormat::depth16:		   return VK_FORMAT_D16_UNORM;
		case GraphicsFormat::depth24Stencil8:  return VK_FORMAT_D24_UNORM_S8_UINT;
		case GraphicsFormat::depth32f:		   return VK_FORMAT_D32_SFLOAT;
		case GraphicsFormat::depth32fStencil8: return VK_FORMAT_D32_SFLOAT_S8_UINT;
	}
}

GraphicsFormat VulkanUtils::getFormat(VkFormat format) {
	switch (format) {
		default:
		case VK_FORMAT_UNDEFINED: return GraphicsFormat::none;

		case VK_FORMAT_R8_UINT:					 return GraphicsFormat::r8;
		case VK_FORMAT_R8_SINT:					 return GraphicsFormat::r8s;
		case VK_FORMAT_R8_UNORM:				 return GraphicsFormat::r8_norm;
		case VK_FORMAT_R8_SNORM:				 return GraphicsFormat::r8s_norm;
		case VK_FORMAT_R16_UINT:				 return GraphicsFormat::r16;
		case VK_FORMAT_R16_SINT:				 return GraphicsFormat::r16s;
		case VK_FORMAT_R16_UNORM:				 return GraphicsFormat::r16_norm;
		case VK_FORMAT_R16_SNORM:				 return GraphicsFormat::r16s_norm;
		case VK_FORMAT_R16_SFLOAT:				 return GraphicsFormat::r16f;
		case VK_FORMAT_R8G8_UINT:				 return GraphicsFormat::rg8;
		case VK_FORMAT_R8G8_SINT:				 return GraphicsFormat::rg8s;
		case VK_FORMAT_R8G8_UNORM:				 return GraphicsFormat::rg8_norm;
		case VK_FORMAT_R8G8_SNORM:				 return GraphicsFormat::rg8s_norm;
		case VK_FORMAT_B4G4R4A4_UNORM_PACK16:	 return GraphicsFormat::bgra4_norm;
		case VK_FORMAT_B5G6R5_UNORM_PACK16:		 return GraphicsFormat::b5g6r5_norm;
		case VK_FORMAT_B5G5R5A1_UNORM_PACK16:	 return GraphicsFormat::b5g5r5a1_norm;
		case VK_FORMAT_R8G8B8A8_UINT:			 return GraphicsFormat::rgba8;
		case VK_FORMAT_R8G8B8A8_SINT:			 return GraphicsFormat::rgba8s;
		case VK_FORMAT_R8G8B8A8_UNORM:			 return GraphicsFormat::rgba8_norm;
		case VK_FORMAT_R8G8B8A8_SNORM:			 return GraphicsFormat::rgba8s_norm;
		case VK_FORMAT_B8G8R8A8_UNORM:			 return GraphicsFormat::bgra8_norm;
		case VK_FORMAT_R8G8B8A8_SRGB:			 return GraphicsFormat::srgba8_norm;
		case VK_FORMAT_B8G8R8A8_SRGB:			 return GraphicsFormat::sbgra8_norm;
		case VK_FORMAT_A2B10G10R10_UNORM_PACK32: return GraphicsFormat::r10g10b10a2_norm;
		case VK_FORMAT_B10G11R11_UFLOAT_PACK32:	 return GraphicsFormat::r11g11b10f;
		case VK_FORMAT_R16G16_UINT:				 return GraphicsFormat::rg16;
		case VK_FORMAT_R16G16_SINT:				 return GraphicsFormat::rg16s;
		case VK_FORMAT_R16G16_UNORM:			 return GraphicsFormat::rg16_norm;
		case VK_FORMAT_R16G16_SNORM:			 return GraphicsFormat::rg16s_norm;
		case VK_FORMAT_R16G16_SFLOAT:			 return GraphicsFormat::rg16f;
		case VK_FORMAT_R32_UINT:				 return GraphicsFormat::r32;
		case VK_FORMAT_R32_SINT:				 return GraphicsFormat::r32s;
		case VK_FORMAT_R32_SFLOAT:				 return GraphicsFormat::r32f;
		case VK_FORMAT_R16G16B16A16_UINT:		 return GraphicsFormat::rgba16;
		case VK_FORMAT_R16G16B16A16_SINT:		 return GraphicsFormat::rgba16s;
		case VK_FORMAT_R16G16B16A16_SFLOAT:		 return GraphicsFormat::rgba16f;
		case VK_FORMAT_R16G16B16A16_UNORM:		 return GraphicsFormat::rgba16_norm;
		case VK_FORMAT_R16G16B16A16_SNORM:		 return GraphicsFormat::rgba16s_norm;
		case VK_FORMAT_R32G32_UINT:				 return GraphicsFormat::rg32;
		case VK_FORMAT_R32G32_SINT:				 return GraphicsFormat::rg32s;
		case VK_FORMAT_R32G32_SFLOAT:			 return GraphicsFormat::rg32f;
		case VK_FORMAT_R32G32B32_UINT:			 return GraphicsFormat::rgb32;
		case VK_FORMAT_R32G32B32_SINT:			 return GraphicsFormat::rgb32s;
		case VK_FORMAT_R32G32B32_SFLOAT:		 return GraphicsFormat::rgb32f;
		case VK_FORMAT_R32G32B32A32_UINT:		 return GraphicsFormat::rgba32;
		case VK_FORMAT_R32G32B32A32_SINT:		 return GraphicsFormat::rgba32s;
		case VK_FORMAT_R32G32B32A32_SFLOAT:		 return GraphicsFormat::rgba32f;
		case VK_FORMAT_D16_UNORM:				 return GraphicsFormat::depth16;
		case VK_FORMAT_D24_UNORM_S8_UINT:		 return GraphicsFormat::depth24Stencil8;
		case VK_FORMAT_D32_SFLOAT:				 return GraphicsFormat::depth32f;
		case VK_FORMAT_D32_SFLOAT_S8_UINT:		 return GraphicsFormat::depth32fStencil8;
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
