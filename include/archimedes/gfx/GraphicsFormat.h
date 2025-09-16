#pragma once
#include <archimedes/ArchMath.h>
#include <archimedes/exceptions/InvalidEnumException.h>

namespace arch::gfx {

enum class GraphicsFormat : u8 {
	none,

	r8,
	r8s,
	r8_norm,
	r8s_norm,

	r16,
	r16s,
	r16_norm,
	r16s_norm,
	r16f,

	rg8,
	rg8s,
	rg8_norm,
	rg8s_norm,

	bgra4_norm,
	b5g6r5_norm,
	b5g5r5a1_norm,
	rgba8,
	rgba8s,
	rgba8_norm,
	rgba8s_norm,
	bgra8_norm,
	srgba8_norm,
	sbgra8_norm,

	r10g10b10a2_norm,
	r11g11b10f,

	rg16,
	rg16s,
	rg16_norm,
	rg16s_norm,
	rg16f,

	r32,
	r32s,
	r32f,

	rgba16,
	rgba16s,
	rgba16f,
	rgba16_norm,
	rgba16s_norm,

	rg32,
	rg32s,
	rg32f,

	rgb32,
	rgb32s,
	rgb32f,

	rgba32,
	rgba32s,
	rgba32f,

	depth16,
	depth24Stencil8,
	depth32f,
	depth32fStencil8,

	_COUNT,
};

inline u64 getPixelSize(GraphicsFormat format) {
	switch (format) {
		case GraphicsFormat::r8:	   return 1;
		case GraphicsFormat::r8s:	   return 1;
		case GraphicsFormat::r8_norm:  return 1;
		case GraphicsFormat::r8s_norm: return 1;

		case GraphicsFormat::r16:		return 2;
		case GraphicsFormat::r16s:		return 2;
		case GraphicsFormat::r16_norm:	return 2;
		case GraphicsFormat::r16s_norm: return 2;
		case GraphicsFormat::r16f:		return 2;
		case GraphicsFormat::rg8:		return 2;
		case GraphicsFormat::rg8s:		return 2;
		case GraphicsFormat::rg8_norm:	return 2;
		case GraphicsFormat::rg8s_norm: return 2;

		case GraphicsFormat::bgra4_norm:	return 4;
		case GraphicsFormat::b5g6r5_norm:	return 4;
		case GraphicsFormat::b5g5r5a1_norm: return 4;
		case GraphicsFormat::rgba8:			return 4;
		case GraphicsFormat::rgba8s:		return 4;

		case GraphicsFormat::rgba8_norm:  return 8;
		case GraphicsFormat::rgba8s_norm: return 8;
		case GraphicsFormat::bgra8_norm:  return 8;
		case GraphicsFormat::srgba8_norm: return 8;
		case GraphicsFormat::sbgra8_norm: return 8;

		case GraphicsFormat::r10g10b10a2_norm: return 4;
		case GraphicsFormat::r11g11b10f:	   return 4;

		case GraphicsFormat::rg16:		 return 8;
		case GraphicsFormat::rg16s:		 return 8;
		case GraphicsFormat::rg16_norm:	 return 8;
		case GraphicsFormat::rg16s_norm: return 8;
		case GraphicsFormat::rg16f:		 return 8;

		case GraphicsFormat::r32:  return 4;
		case GraphicsFormat::r32s: return 4;
		case GraphicsFormat::r32f: return 4;

		case GraphicsFormat::rgba16:	   return 8;
		case GraphicsFormat::rgba16s:	   return 8;
		case GraphicsFormat::rgba16f:	   return 8;
		case GraphicsFormat::rgba16_norm:  return 8;
		case GraphicsFormat::rgba16s_norm: return 8;

		case GraphicsFormat::rg32:	return 8;
		case GraphicsFormat::rg32s: return 8;
		case GraphicsFormat::rg32f: return 8;

		case GraphicsFormat::rgb32:	 return 12;
		case GraphicsFormat::rgb32s: return 12;
		case GraphicsFormat::rgb32f: return 12;

		case GraphicsFormat::rgba32:  return 16;
		case GraphicsFormat::rgba32s: return 16;
		case GraphicsFormat::rgba32f: return 16;

		case GraphicsFormat::depth16:		   return 2;
		case GraphicsFormat::depth24Stencil8:  return 4;
		case GraphicsFormat::depth32f:		   return 4;
		case GraphicsFormat::depth32fStencil8: return 8;

		default: throw InvalidEnumException("Invalid graphics format");
	}
}

} // namespace arch::gfx
