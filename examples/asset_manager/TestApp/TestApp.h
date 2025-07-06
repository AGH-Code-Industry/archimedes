#include <AssetManager.h>

void TestHashing() {
	arch::assetManager::AssetID id1("assets/source/meshes/arrow_triangulated.obj");
	arch::assetManager::AssetID id2("assets/source/shaders/fragment_default.glsl");
	arch::assetManager::AssetID id3("assets/source/textures/example_texture.png");

	arch::Logger::debug("id1: {}", id1.Value());
	arch::Logger::debug("id2: {}", id2.Value());
	arch::Logger::debug("id3: {}", id3.Value());
}

void TestAssetManager() {
	arch::assetManager::AssetImporterManager im("assets/processed");
	im.RegisterImporter(std::make_unique<arch::assetManager::MeshImporter>());
	im.RegisterImporter(std::make_unique<arch::assetManager::ShaderImporter>());

	im.ImportAsset("assets/source/meshes/arrow_triangulated.obj");
	im.ImportAsset("assets/source/shaders/fragment_default2.glsl");
	im.ImportAsset("assets/source/shaders/vertex_default.glsl");
}

