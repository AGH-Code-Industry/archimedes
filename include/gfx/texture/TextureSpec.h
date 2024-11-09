#pragma once

#include <string>

#include "TextureDim.h"
#include "TextureFormat.h"
#include "TextureType.h"
#include "TextureWrapMode.h"
#include "gfx/GraphicsFormat.h"
#include "gfx/resource/SharedResourceFlags.h"
#include "mmath.h"

namespace arch::gfx::texture {

struct TextureSpec {
	uint32_t width = 1;
	uint32_t height = 1;
	uint32_t depth = 1;
	uint32_t arraySize = 1;
	uint32_t mipLevels = 1;
	uint32_t sampleCount = 1;
	uint32_t sampleQuality = 0;
	GraphicsFormat format = GraphicsFormat::none;
	TextureType dimension = TextureType::texture2D;
	std::string debugName;

	bool isShaderResource = false;
	bool isRenderTarget = false;
	bool isUAV = false;
	bool isTypeless = false;
	bool isShadingRateSurface = false;

	resource::SharedResourceFlags sharedResourceFlags = resource::SharedResourceFlags::None;

	// Indicates that the texture is created with no backing memory,
	// and memory is bound to the texture later using bindTextureMemory.
	// On DX12, the texture resource is created at the time of memory binding.
	bool isVirtual = false;
	bool isTiled = false;

	Color clearValue;
	bool useClearValue = false;

	// If keepInitialState is true, command lists that use the texture will automatically
	// begin tracking the texture from the initial state and transition it to the initial state
	// on command list close.
	bool keepInitialState = false;
};

} // namespace arch::gfx::texture
