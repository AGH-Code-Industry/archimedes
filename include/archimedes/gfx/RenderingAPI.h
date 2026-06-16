#pragma once

namespace arch::gfx {

enum class RenderingAPI {
	none = 0,
	vulkan,
	Nvrhi_VK,
	// Nvrhi_DX11,
	// Nvrhi_DX12,
	// directX,
	// metal,
};

} // namespace arch::gfx
