#include "platform/nvrhi/NvrhiUtils.h"

namespace arch::gfx::nvrhi {

::nvrhi::Format NvrhiUtils::getFormat(GraphicsFormat format) {
	switch (format) {
		default:							   return ::nvrhi::Format::UNKNOWN;
		case GraphicsFormat::r8:			   return ::nvrhi::Format::R8_UINT;
		case GraphicsFormat::r8s:			   return ::nvrhi::Format::R8_SINT;
		case GraphicsFormat::r8_norm:		   return ::nvrhi::Format::R8_UNORM;
		case GraphicsFormat::r8s_norm:		   return ::nvrhi::Format::R8_SNORM;
		case GraphicsFormat::r16:			   return ::nvrhi::Format::R16_UINT;
		case GraphicsFormat::r16s:			   return ::nvrhi::Format::R16_SINT;
		case GraphicsFormat::r16_norm:		   return ::nvrhi::Format::R16_UNORM;
		case GraphicsFormat::r16s_norm:		   return ::nvrhi::Format::R16_SNORM;
		case GraphicsFormat::r16f:			   return ::nvrhi::Format::R16_FLOAT;
		case GraphicsFormat::rg8:			   return ::nvrhi::Format::RG8_UINT;
		case GraphicsFormat::rg8s:			   return ::nvrhi::Format::RG8_SINT;
		case GraphicsFormat::rg8_norm:		   return ::nvrhi::Format::RG8_UNORM;
		case GraphicsFormat::rg8s_norm:		   return ::nvrhi::Format::RG8_SNORM;
		case GraphicsFormat::bgra4_norm:	   return ::nvrhi::Format::BGRA4_UNORM;
		case GraphicsFormat::b5g6r5_norm:	   return ::nvrhi::Format::B5G6R5_UNORM;
		case GraphicsFormat::b5g5r5a1_norm:	   return ::nvrhi::Format::B5G5R5A1_UNORM;
		case GraphicsFormat::rgba8:			   return ::nvrhi::Format::RGBA8_UINT;
		case GraphicsFormat::rgba8s:		   return ::nvrhi::Format::RGBA8_SINT;
		case GraphicsFormat::rgba8_norm:	   return ::nvrhi::Format::RGBA8_UNORM;
		case GraphicsFormat::rgba8s_norm:	   return ::nvrhi::Format::RGBA8_SNORM;
		case GraphicsFormat::bgra8_norm:	   return ::nvrhi::Format::BGRA8_UNORM;
		case GraphicsFormat::srgba8_norm:	   return ::nvrhi::Format::SRGBA8_UNORM;
		case GraphicsFormat::sbgra8_norm:	   return ::nvrhi::Format::SBGRA8_UNORM;
		case GraphicsFormat::r10g10b10a2_norm: return ::nvrhi::Format::R10G10B10A2_UNORM;
		case GraphicsFormat::r11g11b10f:	   return ::nvrhi::Format::R11G11B10_FLOAT;
		case GraphicsFormat::rg16:			   return ::nvrhi::Format::RG16_UINT;
		case GraphicsFormat::rg16s:			   return ::nvrhi::Format::RG16_SINT;
		case GraphicsFormat::rg16_norm:		   return ::nvrhi::Format::RG16_UNORM;
		case GraphicsFormat::rg16s_norm:	   return ::nvrhi::Format::RG16_SNORM;
		case GraphicsFormat::rg16f:			   return ::nvrhi::Format::RG16_FLOAT;
		case GraphicsFormat::r32:			   return ::nvrhi::Format::R32_UINT;
		case GraphicsFormat::r32s:			   return ::nvrhi::Format::R32_SINT;
		case GraphicsFormat::r32f:			   return ::nvrhi::Format::R32_FLOAT;
		case GraphicsFormat::rgba16:		   return ::nvrhi::Format::RGBA16_UINT;
		case GraphicsFormat::rgba16s:		   return ::nvrhi::Format::RGBA16_SINT;
		case GraphicsFormat::rgba16f:		   return ::nvrhi::Format::RGBA16_FLOAT;
		case GraphicsFormat::rgba16_norm:	   return ::nvrhi::Format::RGBA16_UNORM;
		case GraphicsFormat::rgba16s_norm:	   return ::nvrhi::Format::RGBA16_SNORM;
		case GraphicsFormat::rg32:			   return ::nvrhi::Format::RG32_UINT;
		case GraphicsFormat::rg32s:			   return ::nvrhi::Format::RG32_SINT;
		case GraphicsFormat::rg32f:			   return ::nvrhi::Format::RG32_FLOAT;
		case GraphicsFormat::rgb32:			   return ::nvrhi::Format::RGB32_UINT;
		case GraphicsFormat::rgb32s:		   return ::nvrhi::Format::RGB32_SINT;
		case GraphicsFormat::rgb32f:		   return ::nvrhi::Format::RGB32_FLOAT;
		case GraphicsFormat::rgba32:		   return ::nvrhi::Format::RGBA32_UINT;
		case GraphicsFormat::rgba32s:		   return ::nvrhi::Format::RGBA32_SINT;
		case GraphicsFormat::rgba32f:		   return ::nvrhi::Format::RGBA32_FLOAT;
		case GraphicsFormat::depth16:		   return ::nvrhi::Format::D16;
		case GraphicsFormat::depth24Stencil8:  return ::nvrhi::Format::D24S8;
		case GraphicsFormat::depth32f:		   return ::nvrhi::Format::D32;
		case GraphicsFormat::depth32fStencil8: return ::nvrhi::Format::D32S8;
	}
}

} // namespace arch::gfx::nvrhi
