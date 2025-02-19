#include "gfx/Renderer.h"

#include "../platform/nvrhi/NvrhiRenderer.h"
#include "../platform/vulkan/VulkanRenderer.h"
#include "Logger.h"

namespace arch::gfx {

Ref<Renderer> Renderer::s_current = nullptr;

Ref<Renderer> Renderer::create(RenderingAPI api) {
	switch (api) {
		// case RenderingAPI::vulkan: return createRef<vulkan::VulkanRenderer>();
		case RenderingAPI::vulkan:
			Logger::critical("Standalone Vulkan is not supported currently. (Use: RenderingAPI::Nvrhi_VK)");
			return nullptr;

		// case RenderingAPI::Nvrhi_DX11:
		// case RenderingAPI::Nvrhi_DX12:
		case RenderingAPI::Nvrhi_VK: return createRef<nvrhi::NvrhiRenderer>(api, true);

		default: Logger::critical("Unknown RenderingAPI {}", (u32)api); return nullptr;
	}
}

Ref<Renderer> Renderer::getCurrent() {
	if (!s_current) {
		Logger::critical("[Renderer] Current context is null");
	}

	return s_current;
}

void Renderer::makeCurrent() {
	s_current = shared_from_this();
}

void Renderer::setClearColor(Color color) {
	_clearColor = color;
}

} // namespace arch::gfx
