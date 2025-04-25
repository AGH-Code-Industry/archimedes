#include <asset_manager/AssetID.h>
#include <asset_manager/importers/AssetImporterManager.h>
#include <asset_manager/importers/MeshImporter.h>
#include <asset_manager/importers/TextureImporter.h>
#include <asset_manager/loaders/MeshLoader.h>
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

void CreateAIM() {
	arch::assetManager::AssetImporterManager manager("assets/processed");
	manager.RegisterImporter(std::make_unique<arch::assetManager::MeshImporter>());

	manager.ImportAsset("assets/source/meshes/arrow_triangulated.obj");
	arch::assetManager::MeshLoader loader{};
	loader.LoadFromFile("assets/processed/meshes/arrow_triangulated.archmesh");
}
