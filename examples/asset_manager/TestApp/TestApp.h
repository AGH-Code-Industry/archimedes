#include <AssetManager.h>

//!			TODO (Asset Manager):
// x			Redesign importing (user has to pass asset type as an argument)
// x			Path mirroring
// x			Loading paths
// todo		Exceptions -> Optionals
// todo		Easier access (more comfortable) to registering importers and especially loaders
// todo		Easier access (more comfortable) to creating load requests
// todo		Textures (and everything connected to them)
// todo		Sounds (and everything connected to them)

void TestHashing() {
	arch::assetManager::AssetID id1("assets/source/meshes/arrow_triangulated.obj");
	arch::assetManager::AssetID id2("assets/source/shaders/fragment_default.glsl");
	arch::assetManager::AssetID id3("assets/source/textures/example_texture.png");

	arch::Logger::debug("id1: {}", id1.Value());
	arch::Logger::debug("id2: {}", id2.Value());
	arch::Logger::debug("id3: {}", id3.Value());
}

void TestAssetManager() {
	arch::assetManager::AssetImporterManager im("assets/source", "assets/processed");
	im.RegisterImporter(std::make_unique<arch::assetManager::MeshImporter>());
	im.RegisterImporter(std::make_unique<arch::assetManager::ShaderImporter>());

	im.ImportAsset("meshes/arrow_triangulated", arch::assetManager::AssetType::MESH);

	im.SetShaderImportSettings(
		arch::assetManager::ShaderOptimizationMode::PERFORMANCE,
		arch::assetManager::ShaderType::FRAGMENT,
		arch::assetManager::ShaderSourceLanguage::GLSL
	);
	im.ImportAsset("shaders/fragment_default2", arch::assetManager::AssetType::SHADER);
	im.SetShaderImportSettings(
		arch::assetManager::ShaderOptimizationMode::SIZE,
		arch::assetManager::ShaderType::VERTEX,
		arch::assetManager::ShaderSourceLanguage::GLSL
	);
	im.ImportAsset("shaders/vertex_default", arch::assetManager::AssetType::SHADER);

	arch::assetManager::AssetManager am;
	am.RegisterLoader<arch::assetManager::assets::Mesh>(std::make_unique<arch::assetManager::MeshLoader>("assets/processed"));
	am.RegisterLoader<arch::assetManager::assets::Shader>(
		std::make_unique<arch::assetManager::ShaderLoader>("assets/processed")
	);

	auto mesh = am.LoadAsync<arch::assetManager::assets::Mesh>(
		"meshes/arrow_triangulated"
	);

	arch::Logger::debug("Mesh status: {}", mesh.IsReady() ? "Ready" : "Loading");
	am.TickLoader();
	arch::Logger::debug("Mesh status: {}", mesh.IsReady() ? "Ready" : "Loading");

	auto shader = am.LoadAsync<arch::assetManager::assets::Shader>(
		"shaders/vertex_default"
	);

	arch::Logger::debug("Shader status: {}", shader.IsReady() ? "Ready" : "Loading");
	am.TickLoader();
	arch::Logger::debug("Shader status: {}", shader.IsReady() ? "Ready" : "Loading");

	auto mesh2 = am.LoadAsync<arch::assetManager::assets::Mesh>("meshes/arrow_triangulated");
	auto shader2 = am.LoadAsync<arch::assetManager::assets::Shader>("shaders/vertex_default");
	am.TickLoader();

	arch::Logger::debug("Shader2 status: {}", shader.IsReady() ? "Ready" : "Loading");
	arch::Logger::debug("Mesh2 status: {}", mesh.IsReady() ? "Ready" : "Loading");

}

