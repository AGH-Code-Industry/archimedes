#include <iostream>

#include <asset_manager/MeshImporter.h>

namespace arch::assetManager {

bool MeshImporter::SupportsFile(const std::filesystem::path& path) const {
	std::string extension{ path.extension().string() };
	if (extension == ".obj") {
		arch::Logger::trace("File format verified.");
		return true;
	} else {
		arch::Logger::warn("File format not supported.");
		return false;
	}
}

void MeshImporter::Import(const std::filesystem::path& sourcePath, const std::filesystem::path& processedPath) const {
	std::ifstream inStream(sourcePath);

	if (!inStream) {
		arch::Logger::error(".obj file not found");
		return;
	}

	std::string line{};

	std::vector<float> vertices{};
	std::vector<float> uvs{};
	std::vector<size_t> faceIndices{};

	while (std::getline(inStream, line)) {
		std::istringstream iss(line);
		std::string prefix{};
		iss >> prefix;

		float x, y, u, v;

		if (prefix == "v") {
			iss >> x >> y;
			vertices.emplace_back(x);
			vertices.emplace_back(y);
		} else if (prefix == "vt") {
			iss >> u >> v;
			uvs.emplace_back(u);
			uvs.emplace_back(v);
		} else if (prefix == "f") {
			for (size_t i{}; i < 3; i++) {
				std::string l{};
				iss >> l;

				size_t firstSlash{ l.find('/') };
				size_t secondSlash{ l.find('/', firstSlash + 1) };

				size_t vIndex;
				size_t vtIndex;

				faceIndices.emplace_back(static_cast<size_t>(std::stoi(l.substr(0, firstSlash))) - 1);
				faceIndices.emplace_back(
					static_cast<size_t>(std::stoi(l.substr(firstSlash + 1, secondSlash - firstSlash - 1))) - 1
				);
			}
		}
	}

	inStream.close();

	bool hasUVs{ !uvs.empty() };
	std::vector<float> finalVertices{};
	std::vector<uint16_t> finalIndices{};

	std::map<std::pair<size_t, size_t>, uint16_t> vertexToIndex{};

	for (size_t i{}; i < faceIndices.size(); i += 2) {
		std::pair<size_t, size_t> key{ faceIndices.at(i), faceIndices.at(i + 1) };
		uint16_t index{};
		if (vertexToIndex.count(key) != 0) {
			index = vertexToIndex[key];
		} else {
			index = finalVertices.size() / 4;
			if (hasUVs) {
				finalVertices.emplace_back(vertices.at(faceIndices.at(i) * 2));
				finalVertices.emplace_back(vertices.at(faceIndices.at(i) * 2 + 1));
				finalVertices.emplace_back(uvs.at(faceIndices.at(i + 1) * 2));
				finalVertices.emplace_back(uvs.at(faceIndices.at(i + 1) * 2 + 1));
			} else {
				finalVertices.emplace_back(vertices.at(faceIndices.at(i) * 2));
				finalVertices.emplace_back(vertices.at(faceIndices.at(i) * 2 + 1));
				finalVertices.emplace_back(-1.0f);
				finalVertices.emplace_back(-1.0f);
			}
			vertexToIndex[key] = index;
		}
		finalIndices.emplace_back(index);
	}

	/*for (size_t i{}; i < finalVertices.size(); i += 4) {
		std::cout << finalVertices.at(i) << " " << finalVertices.at(i + 1) << " " << finalVertices.at(i + 2) << " "
				  << finalVertices.at(i + 3) << std::endl;
	}*/

	std::ofstream outStream(processedPath.string() + "/" + sourcePath.stem().string() + ".archmesh", std::ios::binary);

	if (!outStream) {
		arch::Logger::error("Mesh file wasn't created");
		return;
	}

	const char magic[4]{ 'M', 'S', 'H', 'B' };
	outStream.write(reinterpret_cast<const char*>(&magic), sizeof(magic));

	uint16_t version{ 1 };
	outStream.write(reinterpret_cast<const char*>(&version), sizeof(version));

	uint16_t flags{ static_cast<uint16_t>(hasUVs ? 1 : 0) };
	outStream.write(reinterpret_cast<const char*>(&flags), sizeof(flags));

	uint32_t vertexCount{ static_cast<uint32_t>(finalVertices.size() / 4) };
	outStream.write(reinterpret_cast<const char*>(&vertexCount), sizeof(vertexCount));

	uint32_t indexCount{ static_cast<uint32_t>(finalIndices.size()) };
	outStream.write(reinterpret_cast<const char*>(&indexCount), sizeof(indexCount));

	uint32_t vertexSize{ 16 };
	outStream.write(reinterpret_cast<const char*>(&vertexSize), sizeof(vertexSize));

	uint32_t vertexDataOffset{ 32 };
	outStream.write(reinterpret_cast<const char*>(&vertexDataOffset), sizeof(vertexDataOffset));

	uint32_t indexDataOffset{ vertexDataOffset + vertexCount * vertexSize };
	outStream.write(reinterpret_cast<const char*>(&indexDataOffset), sizeof(indexDataOffset));

	uint32_t reserved{};
	outStream.write(reinterpret_cast<const char*>(&reserved), sizeof(reserved));

	for (float& i : finalVertices) {
		outStream.write(reinterpret_cast<const char*>(&i), sizeof(i));
	}

	for (uint16_t& i : finalIndices) {
		outStream.write(reinterpret_cast<const char*>(&i), sizeof(i));
	}

	outStream.close();
}

} // namespace arch::assetManager
