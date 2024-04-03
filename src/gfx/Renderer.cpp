#include "gfx/Renderer.h"

#include "Logger.h"
#include "platform/vulkan/VulkanRenderer.h"

namespace arch::gfx {

Ref<Renderer> Renderer::current = nullptr;

Ref<Renderer> Renderer::create(RenderingAPI api) {
	switch (api) {
		case RenderingAPI::vulkan: return std::make_shared<vulkan::VulkanRenderer>();

		default: Logger::critical("Unknown RenderingAPI {}", (u32)api); return nullptr;
	}
	return nullptr;
}

Ref<Renderer> Renderer::getCurrent() {
	if (!current) {
		Logger::critical("[Renderer] Current context is null");
	}

	return current;
}

void Renderer::makeCurrent() {
	current = shared_from_this();
}

} // namespace arch::gfx
