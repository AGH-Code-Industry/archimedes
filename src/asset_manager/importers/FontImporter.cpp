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
	std::ifstream inStream(filePath, std::ios::binary | std::ios::ate);

	if (!inStream) {
		arch::Logger::error("File not found - '{}'", sourcePath.string());
		return;
	} else {
		arch::Logger::trace("File succesfully located - '{}'", filePath.string());
	}

	std::streamsize size = inStream.tellg();
	inStream.seekg(0, std::ios::beg);

	std::vector<char> buffer(size);

	if (!inStream.read(buffer.data(), size)) {
		arch::Logger::error("Unable to read file - '{}'", sourcePath.string());
	} else {
		arch::Logger::trace("File succesfully read ('{}')", filePath.string());
	}

	FT_Library ftLibrary;
	if (FT_Init_FreeType(&ftLibrary)) {
		arch::Logger::error("Failed to initialise FreeType");
	}

	FT_Face ftFace;
	if (FT_New_Memory_Face(ftLibrary, reinterpret_cast<const FT_Byte*>(buffer.data()), buffer.size(), 0, &ftFace)) {
		arch::Logger::error("Failed to create font face.");
	}

	if (FT_Set_Pixel_Sizes(ftFace, 0, 64)) {
		arch::Logger::error("Failed to set pixel sizes");
	}

	std::string charset{
		" !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"
	};




} // namespace arch::assetManager
