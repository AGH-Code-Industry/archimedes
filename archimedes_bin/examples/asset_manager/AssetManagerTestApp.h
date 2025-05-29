#include <AssetManager.h>
#include <iostream>

void TestHashing() {
	std::string examplePath1{ "/mnt/test/path/asset.png" };
	arch::assetManager::AssetID id1(examplePath1);

	std::string examplePath2{ "/mnt/test/path/further/asset.mp3" };
	arch::assetManager::AssetID id2(examplePath2);

	std::cout << "Asset 1 ID: " << std::hex << id1.Value() << std::endl;
	std::cout << "Asset 2 ID: " << std::hex << id2.Value() << std::endl;

	if (id1 == id2) {
		std::cout << "IDs are equal" << std::endl;
	} 

	if (id1 != id2) {
		std::cout << "IDs are different" << std::endl;
	}
}

void TestImporting() {
	arch::assetManager::AssetImporterManager manager("assets/processed");
	manager.RegisterImporter(std::make_unique<arch::assetManager::MeshImporter>());
	manager.ImportAsset("assets/source/meshes/arrow_triangulated.obj");
	manager.RegisterImporter(std::make_unique<arch::assetManager::ShaderImporter>());
	manager.ImportAsset("assets/source/shaders/fragment_default2.glsl");
	manager.ImportAsset("assets/source/shaders/vertex_default.glsl");

}

