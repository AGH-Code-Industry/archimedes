#include "asset_manager/loaders/TextureLoader.h"

namespace arch::assetManager {
	TextureLoader::TextureLoader() {
		arch::Logger::info("Texture loader created.");
	}

	std::shared_ptr<arch::assetManager::assets::Texture> TextureLoader::LoadFromFile(const std::filesystem::path& path
	) const {

		// TODO: Load png file as texture
		// TODO: Generate mipmaps
		// TODO: Create buffers.

		return std::shared_ptr<arch::assetManager::assets::Texture>();
	}

}

