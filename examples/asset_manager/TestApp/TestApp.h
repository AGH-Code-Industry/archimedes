#include <AssetManager.h>

//!			TODO (Asset Manager):
// x			Redesign importing (user has to pass asset type as an argument)
// x			Path mirroring
// x			Loading paths
// x			Exceptions -> Optionals
// x			Easier access (more comfortable) to registering importers and especially loaders
// x			Easier access (more comfortable) to creating load requests
// todo		Text (and everything connected to it)
// todo		Textures (and everything connected to them)
// todo		Sounds (and everything connected to them)
// todo		Documentation

void TestHashing() {
	using namespace arch::assetManager;

	AssetID id1("assets/source/meshes/arrow_triangulated.obj");
	AssetID id2("assets/source/shaders/fragment_default.glsl");
	AssetID id3("assets/source/textures/example_texture.png");

	arch::Logger::debug("id1: {}", id1.Value());
	arch::Logger::debug("id2: {}", id2.Value());
	arch::Logger::debug("id3: {}", id3.Value());
}

void TestAssetManager() {
	using namespace arch::assetManager;

	AssetImporterManager im("assets/source", "assets/processed");
	im.Register<MeshImporter>();
	im.Register<ShaderImporter>();

	im.ImportAsset("meshes/arrow_triangulated", AssetType::MESH);

	im.SetShaderImportSettings(ShaderOptimizationMode::PERFORMANCE, ShaderType::FRAGMENT, ShaderSourceLanguage::GLSL);
	im.ImportAsset("shaders/fragment_default2", AssetType::SHADER);
	im.SetShaderImportSettings(ShaderOptimizationMode::SIZE, ShaderType::VERTEX, ShaderSourceLanguage::GLSL);
	im.ImportAsset("shaders/vertex_default", AssetType::SHADER);

	AssetManager am;
	am.Register<assets::Mesh, MeshLoader>("assets/processed");
	am.Register<assets::Shader, ShaderLoader>("assets/processed");

	auto mesh = am.LoadAsync<assets::Mesh>("meshes/arrow_triangulated");

	arch::Logger::debug("Mesh status: {}", mesh.IsReady() ? "Ready" : "Loading");
	am.TickLoader();
	arch::Logger::debug("Mesh status: {}", mesh.IsReady() ? "Ready" : "Loading");

	auto shader = am.LoadAsync<assets::Shader>("shaders/vertex_default");

	arch::Logger::debug("Shader status: {}", shader.IsReady() ? "Ready" : "Loading");
	am.TickLoader();
	arch::Logger::debug("Shader status: {}", shader.IsReady() ? "Ready" : "Loading");

	auto mesh2 = am.LoadAsync<assets::Mesh>("meshes/arrow_triangulated");
	auto shader2 = am.LoadAsync<assets::Shader>("shaders/vertex_default");
	am.TickLoader();

	arch::Logger::debug("Shader2 status: {}", shader.IsReady() ? "Ready" : "Loading");
	arch::Logger::debug("Mesh2 status: {}", mesh.IsReady() ? "Ready" : "Loading");
}

