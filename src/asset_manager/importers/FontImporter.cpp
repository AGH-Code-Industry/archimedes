#include <asset_manager/importers/FontImporter.h>

namespace arch::assetManager {

bool FontImporter::SupportsFile(const std::filesystem::path& path) const noexcept {
	std::string extension{ path.extension().string() };
	if (extension == ".ttf") {
		return true;
	} else {
		return false;
	}
}

void FontImporter::Import(
	const std::filesystem::path& sourcePath,
	const std::filesystem::path& processedPath,
	const std::filesystem::path& filePath
) const noexcept {
	std::array<std::string, 12> args{};

	// Font file
	args[0] = "-font \"" + filePath.string() + "\"";

	// Charset
	args[1] = "-chars \"[0x20, 0xFF], [0x100, 0x17F], [0x2018, 0x201E], 0x20AC\"";

	// EO + 80 + 7 + 1

	// Atlas gen type
	args[2] = "-type msdf";

	// Generation accuracy
	args[3] = "-size 64";

	// Margin
	args[4] = "-pxrange 8";

	// Correction algorithm
	args[5] = "-errorcorrection distance";

	// Auto square size
	args[6] = "-pots";

	// 0,0 in top left
	args[7] = "-yorigin top";

	// out format
	args[8] = "-format binfloat";

	if (!std::filesystem::exists(processedPath)) {
		std::filesystem::create_directories(processedPath);
	}

	std::string assetPath{ processedPath.string() + "/" + sourcePath.string() };
	std::filesystem::create_directories(assetPath);

	std::string binPath{ assetPath + "/" + filePath.stem().string() + ".bin" };
	std::string jsonPath{ assetPath + "/" + filePath.stem().string() + ".json" };

	// Out image
	args[9] = "-imageout \"" + binPath + "\"";

	// JSON out
	args[10] = "-json \"" + jsonPath + "\"";

	// Amount of threads to use during generation
	args[11] = "-threads 0";

	std::string command{ std::format("{} ", arch::font::MSDFAtlasGen::get().path()) };

	for (size_t i{}; i < args.size(); ++i) {
		command += args.at(i) + " ";
	}

	std::string outputMuter = ARCHIMEDES_WINDOWS ? "nul" : "/dev/null";

	command += std::format("> {0} 2> {0}", outputMuter);

	arch::Logger::trace("Launching msdf-atlas-gen on {}", filePath.string());
	arch::Logger::trace("Running: {}", command);
	if (std::system(command.c_str())) {
		arch::Logger::error("Unable to perform atlas generation ('{}'). Aborting import.", filePath.string());
		return;
	}
	arch::Logger::trace("Succesfully processed {}", filePath.string());

	Json::Value root{};

	std::ifstream jsonFile(jsonPath, std::ios::binary);

	if (!jsonFile.is_open()) {
		arch::Logger::error("Failed to open '{}'. Aborting import.", jsonPath);
		return;
	}

	try {
		jsonFile >> root;
	} catch (Json::Exception& error) {
		arch::Logger::error("Error during JSON parsing ('{}'). Aborting import.", jsonPath);
		return;
	}

	assets::AtlasMetadata atlasMeta{};
	atlasMeta.width = root["atlas"]["width"].asUInt();
	atlasMeta.height = root["atlas"]["height"].asUInt();
	atlasMeta.format = 24;

	assets::FontMetrics fontMetrics{};
	fontMetrics.lineHeight = root["metrics"]["lineHeight"].asFloat();
	fontMetrics.ascender = root["metrics"]["ascender"].asFloat();
	fontMetrics.descender = root["metrics"]["descender"].asFloat();

	size_t glyphCount{ 360 };
	std::vector<assets::GlyphData> glyphs;
	glyphs.reserve(glyphCount);

	const Json::Value& jsonGlyphs{ root["glyphs"] };

	for (const Json::Value& glyphNode : jsonGlyphs) {
		assets::GlyphData glyphData;

		glyphData.unicode = glyphNode["unicode"].asUInt();
		glyphData.advance = glyphNode["advance"].asFloat();

		if (glyphNode.isMember("planeBounds")) {
			glyphData.planeLeft = glyphNode["planeBounds"]["left"].asFloat();
			glyphData.planeBottom = glyphNode["planeBounds"]["bottom"].asFloat();
			glyphData.planeRight = glyphNode["planeBounds"]["right"].asFloat();
			glyphData.planeTop = glyphNode["planeBounds"]["top"].asFloat();
		}

		if (glyphNode.isMember("atlasBounds")) {
			// Normalizing pixel coords
			glyphData.atlasLeft = glyphNode["atlasBounds"]["left"].asFloat() / atlasMeta.width;
			glyphData.atlasBottom = glyphNode["atlasBounds"]["bottom"].asFloat() / atlasMeta.height;
			glyphData.atlasRight = glyphNode["atlasBounds"]["right"].asFloat() / atlasMeta.width;
			glyphData.atlasTop = glyphNode["atlasBounds"]["top"].asFloat() / atlasMeta.height;
		}

		glyphs.emplace_back(glyphData);
	}

	std::vector<assets::KerningPair> kerningPairs;
	const Json::Value& jsonKerning = root["kerning"];

	if (jsonKerning.isArray()) {
		kerningPairs.reserve(jsonKerning.size());
	}

	for (const auto& kerningNode : jsonKerning) {
		assets::KerningPair kerningPair;
		kerningPair.unicode1 = kerningNode["unicode1"].asUInt();
		kerningPair.unicode2 = kerningNode["unicode2"].asUInt();
		kerningPair.advanceCorrection = kerningNode["advance"].asFloat();
		kerningPairs.emplace_back(kerningPair);
	}

	jsonFile.close();

	std::vector<uint8_t> pixelData;
	std::ifstream binFile(binPath, std::ios::binary);

	if (!binFile.is_open()) {
		arch::Logger::error("Failed to open '{}'. Aborting import.", binPath);
		return;
	}

	std::streamsize size{ binFile.tellg() };
	binFile.seekg(0, std::ios::beg);

	pixelData.resize(size);

	if (!binFile.read(reinterpret_cast<char*>(pixelData.data()), size)) {
		arch::Logger::error("Failed to read from '{}'. Aborting import.", binPath);
		return;
	}

	binFile.close();

	std::sort(glyphs.begin(), glyphs.end(), [](const auto& a, const auto& b) { return a.unicode < b.unicode; });

	std::sort(kerningPairs.begin(), kerningPairs.end(), [](const auto& a, const auto& b) {
		if (a.unicode1 != b.unicode1) {
			return a.unicode1 < b.unicode1;
		}
		return a.unicode2 < b.unicode2;
	});

	assets::FontAssetHeader header;
	header.magic = 0x464F'4E54; // 'FONT'
	header.version = 1;
	header.glyphCount = glyphs.size();
	header.kerningPairCount = kerningPairs.size();

	uint32_t currentOffset{ sizeof(assets::FontAssetHeader) };
	header.metricsOffset = currentOffset;
	currentOffset += sizeof(assets::FontMetrics);
	header.atlasMetaOffset = currentOffset;
	currentOffset += sizeof(assets::AtlasMetadata);
	header.glyphsOffset = currentOffset;
	currentOffset += glyphs.size() * sizeof(assets::GlyphData);
	header.kerningOffset = currentOffset;
	currentOffset += kerningPairs.size() * sizeof(assets::KerningPair);
	header.pixelsOffset = currentOffset;

	std::ofstream outFile(assetPath + "/" + filePath.stem().string() + ".archfont", std::ios::binary);
	if (!outFile.is_open()) {
		arch::Logger::error(
			"Failed to create archfont file ('{}'). Aborting import.",
			assetPath + "/" + filePath.stem().string() + ".archfont"
		);
		return;
	}

	outFile.write(reinterpret_cast<const char*>(&header), sizeof(header));
	outFile.write(reinterpret_cast<const char*>(&fontMetrics), sizeof(fontMetrics));
	outFile.write(reinterpret_cast<const char*>(&atlasMeta), sizeof(atlasMeta));
	outFile.write(reinterpret_cast<const char*>(glyphs.data()), glyphs.size() * sizeof(assets::GlyphData));
	outFile.write(
		reinterpret_cast<const char*>(kerningPairs.data()),
		kerningPairs.size() * sizeof(assets::KerningPair)
	);
	outFile.write(reinterpret_cast<const char*>(pixelData.data()), pixelData.size());

	outFile.close();

	try {
		if (std::filesystem::exists(jsonPath)) {
			std::filesystem::remove(jsonPath);
		}
		if (std::filesystem::exists(binPath)) {
			std::filesystem::remove(binPath);
		}
		arch::Logger::trace("Temporary font files removed.");

	} catch (const std::filesystem::filesystem_error& e) {
		arch::Logger::warn("Temporary font files couldn't be removed: {}", e.what());
	}

}

} // namespace arch::assetManager
