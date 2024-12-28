#include "VulkanTextureManager.h"

#include "../VulkanRenderer.h"
#include "../texture/VulkanTexture.h"

namespace arch::gfx::vulkan::texture {

VulkanTextureManager::VulkanTextureManager(const Ref<VulkanContext>& context): TextureManager(), _context(context) {}

Ref<Texture> VulkanTextureManager::_createTexture2DImpl(
	u32 width,
	u32 height,
	GraphicsFormat format,
	void* data,
	TextureWrapMode wrapMode,
	TextureFilterMode filterMode,
	bool isReadable
) {
	Ref<VulkanTexture> texture = createRef<VulkanTexture>(
		_context,
		uint3{ width, height, 1 },
		TextureType::texture2D,
		format,
		wrapMode,
		filterMode,
		isReadable
	);

	return texture;
}

} // namespace arch::gfx::vulkan::texture
