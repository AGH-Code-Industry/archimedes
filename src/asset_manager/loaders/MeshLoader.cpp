#include <asset_manager/loaders/MeshLoader.h>

namespace arch::assetManager {

MeshLoader::MeshLoader(std::filesystem::path processedPath) : _processedPath(processedPath) {
	arch::Logger::trace("Mesh Loader created");
}

std::shared_ptr<arch::assetManager::assets::Mesh> MeshLoader::LoadFromFile(const std::filesystem::path& path) const {
	size_t currentOffset{};

	std::string finalPath{ _processedPath.string() + "/" + path.string() + ".archmesh" };

	std::ifstream inStream{ finalPath };

	if (!std::filesystem::exists(finalPath)) {
		arch::Logger::error("'{}' not found", finalPath);
		throw AssetException("Processed asset not found.");
	}

	if (!inStream) {
		arch::Logger::error("Cannot open '{}'", finalPath);
		throw AssetException("Cannot open processed asset.");
	}

	std::array<char, 4> magic{};
	inStream.read(magic.data(), magic.size());
	currentOffset += magic.size();

	constexpr std::string_view expectedMagic{ "MSHB", 4 };
	if (std::string_view{ magic.data(), magic.size() } != expectedMagic) {
		arch::Logger::error("Wrong magic (expected MSHB). Processed asset type not correct.");
		throw AssetException("Wrong magic (expected MSHB). Processed asset type not correct.");
	}

	uint16_t version{};
	inStream.read(reinterpret_cast<char*>(&version), sizeof(version));
	currentOffset += sizeof(version);
	if (version != 1) {
		arch::Logger::error("Processed asset format version {} - not supported", version);
		throw AssetException("Processed asset format version not supported.");
	}

	uint16_t hasUVs{};
	inStream.read(reinterpret_cast<char*>(&hasUVs), sizeof(hasUVs));
	currentOffset += sizeof(hasUVs);

	uint32_t vertexCount{};
	inStream.read(reinterpret_cast<char*>(&vertexCount), sizeof(vertexCount));
	currentOffset += sizeof(vertexCount);
	if (vertexCount == 0) {
		arch::Logger::warn("No vertices in mesh.");
	}

	uint32_t indexCount{};
	inStream.read(reinterpret_cast<char*>(&indexCount), sizeof(indexCount));
	currentOffset += sizeof(indexCount);
	if (vertexCount == 0) {
		arch::Logger::warn("No indices in mesh.");
	}

	uint32_t vertexSize{};
	inStream.read(reinterpret_cast<char*>(&vertexSize), sizeof(vertexSize));
	currentOffset += sizeof(vertexSize);
	if (vertexSize == 0) {
		arch::Logger::error("Vertex size set to 0.");
		throw AssetException("Vertex size set to 0.");
	}

	uint32_t vertexDataOffset{};
	inStream.read(reinterpret_cast<char*>(&vertexDataOffset), sizeof(vertexDataOffset));
	currentOffset += sizeof(vertexDataOffset);
	if (vertexDataOffset != 32) {
		arch::Logger::warn("Vertex Data Offset other than 32 ({}).", vertexDataOffset);
	}

	uint32_t indexDataOffset{};
	inStream.read(reinterpret_cast<char*>(&indexDataOffset), sizeof(indexDataOffset));
	currentOffset += sizeof(indexDataOffset);
	if (indexDataOffset != vertexDataOffset + vertexCount * vertexSize) {
		arch::Logger::warn("Index Data Offset different than expected ({}).", vertexDataOffset);
	}

	inStream.seekg(4, std::ios::cur);
	currentOffset += 4;

	if (currentOffset != 32) {
		arch::Logger::warn("Header offset different than expected ({}).", currentOffset);
	}

	arch::Logger::trace("Asset passed verification ('{}').", finalPath);

	std::vector<float> vertices(4 * vertexCount);
	inStream.read(reinterpret_cast<char*>(vertices.data()), vertexCount * 4 * sizeof(float));

	std::vector<uint32_t> indices(indexCount);
	inStream.read(reinterpret_cast<char*>(indices.data()), indexCount * sizeof(uint32_t));

	std::shared_ptr<arch::assetManager::assets::Mesh> mesh{
		std::make_shared<arch::assetManager::assets::Mesh>(vertices, indices)
	};
	return mesh;
}

} // namespace arch::assetManager
