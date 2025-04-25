#include <asset_manager/loaders/MeshLoader.h>

namespace arch::assetManager {

MeshLoader::MeshLoader() {
	arch::Logger::info("Mesh Loader created");
}

std::shared_ptr<Mesh> MeshLoader::LoadFromFile(const std::filesystem::path& path) const {
	size_t currentOffset{};
	
	std::ifstream inStream{ path };

	if (!std::filesystem::exists(path)) {
		arch::Logger::error("Processed asset not found.");
		return std::make_shared<Mesh>();
	}

	if (!inStream) {
		arch::Logger::error("Cannot open processed asset.");
		return std::make_shared<Mesh>();
	}

	std::array<char, 4> magic{};
	inStream.read(magic.data(), magic.size());
	currentOffset += magic.size();

	constexpr std::string_view expectedMagic{ "MSHB", 4 };
	if (std::string_view{ magic.data(), magic.size() } != expectedMagic) {
		arch::Logger::error("Wrong magic. Processed asset type not correct.");
		return std::make_shared<Mesh>();
	}

	uint16_t version{};
	inStream.read(reinterpret_cast<char*>(&version), sizeof(version));
	currentOffset += sizeof(version);
	if (version != 1) {
		arch::Logger::error("Processed asset format version not supported.");
		return std::make_shared<Mesh>();
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
		return std::make_shared<Mesh>();
	}

	uint32_t vertexDataOffset{};
	inStream.read(reinterpret_cast<char*>(&vertexDataOffset), sizeof(vertexDataOffset));
	currentOffset += sizeof(vertexDataOffset);
	if (vertexDataOffset != 32) {
		arch::Logger::warn("Vertex Data Offset other than 32");
	}

	uint32_t indexDataOffset{};
	inStream.read(reinterpret_cast<char*>(&indexDataOffset), sizeof(indexDataOffset));
	currentOffset += sizeof(indexDataOffset);
	if (indexDataOffset != vertexDataOffset + vertexCount * vertexSize) {
		arch::Logger::warn("Index Data Offset different than expected");
	}

	inStream.seekg(4, std::ios::cur);
	currentOffset += 4;

	if (currentOffset != 32) {
		arch::Logger::warn("Header offset different than expected.");
	}
	 
	arch::Logger::trace("Asset passed verification.");
	
	std::vector<float> vertices(4 * vertexCount);
	inStream.read(reinterpret_cast<char*>(vertices.data()), vertexCount * 4 * sizeof(float));
	
	std::vector<uint16_t> indices(indexCount);
	inStream.read(reinterpret_cast<char*>(indices.data()), indexCount * sizeof(uint16_t));
	
	arch::Logger::trace("Asset loaded");
	return std::make_shared<Mesh>();
}

} // namespace arch::assetManager
