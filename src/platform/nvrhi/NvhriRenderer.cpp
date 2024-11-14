#include "Logger.h"
#include "platform/nvrhi/NvrhiRenderer.h"
#include "platform/nvrhi/context/NvrhiVulkanContext.h"

namespace arch::gfx::nvrhi {

NvrhiRenderer::NvrhiRenderer(RenderingAPI api): Renderer(api) {}

void NvrhiRenderer::init(const Ref<Window>& window) {
	switch (_api) {
		case RenderingAPI::Nvrhi_VK:
			{
				_context = createRef<NvrhiVulkanContext>();
				break;
			}
		// case RenderingAPI::Nvrhi_DX11: break;
		// case RenderingAPI::Nvrhi_DX12: break;
		default: Logger::error("Invalid rendering API"); break;
	}
	_context->init(window);
}

void NvrhiRenderer::shutdown() {}

void NvrhiRenderer::prepareFrame() {}

void NvrhiRenderer::beginFrame() {}

void NvrhiRenderer::endFrame() {}

void NvrhiRenderer::present() {}

void NvrhiRenderer::setClearColor(Color color) {}

void NvrhiRenderer::setViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {}

void NvrhiRenderer::setScissor(uint32_t x, uint32_t y, uint32_t width, uint32_t height) {}

void NvrhiRenderer::render(const Ref<Mesh>& mesh, const Mat4x4& transform) {}

Ref<buffer::BufferManager> NvrhiRenderer::getBufferManager() {
	return nullptr;
}

Ref<texture::TextureManager> NvrhiRenderer::getTextureManager() {
	return nullptr;
}

} // namespace arch::gfx::nvrhi
