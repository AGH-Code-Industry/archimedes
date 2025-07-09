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

	im.SetShaderImportSettings(
		arch::assetManager::ShaderOptimizationMode::PERFORMANCE,
		arch::assetManager::ShaderType::FRAGMENT,
		arch::assetManager::ShaderSourceLanguage::GLSL
	);
	im.ImportAsset("assets/source/shaders/fragment_default2.glsl");
	im.SetShaderImportSettings(
		arch::assetManager::ShaderOptimizationMode::SIZE,
		arch::assetManager::ShaderType::VERTEX,
		arch::assetManager::ShaderSourceLanguage::GLSL
	);
	im.ImportAsset("assets/source/shaders/vertex_default.glsl");

	arch::assetManager::AssetManager am;
	am.RegisterLoader<arch::assetManager::assets::Mesh>(std::make_unique<arch::assetManager::MeshLoader>());
	am.RegisterLoader<arch::assetManager::assets::Shader>(std::make_unique<arch::assetManager::ShaderLoader>());

	auto mesh = am.LoadAsync<arch::assetManager::assets::Mesh>("assets/processed/meshes/arrow_triangulated.archmesh");

	
	arch::Logger::debug("Mesh status: {}", mesh.IsReady() ? "Ready" : "Loading");
	am.TickLoader();
	arch::Logger::debug("Mesh status: {}", mesh.IsReady() ? "Ready" : "Loading");

	
}

