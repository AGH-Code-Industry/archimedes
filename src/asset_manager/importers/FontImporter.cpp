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

	std::string assetPath = processedPath.string() + "/" + sourcePath.string();
	std::filesystem::create_directories(assetPath);

	// Out image
	args[9] = "-imageout \"" + assetPath + "/" + filePath.stem().string() + ".bin" + "\"";

	// JSON out
	args[10] = "-json \"" + assetPath + "/" + filePath.stem().string() + ".json" + "\"";

	// Amount of threads to use during generation
	args[11] = "-threads 0";

	std::string command{ std::format("{} ", arch::font::MSDFAtlasGen::get().path())};

	for (size_t i{}; i < args.size(); ++i) {
		command += args.at(i) + " ";
	}
	
	std::string outputMuter = ARCHIMEDES_WINDOWS ? "nul" : "/dev/null";

	command += std::format("> {0} 2> {0}", outputMuter);



	arch::Logger::trace("Launching msdf-atlas-gen on {}", filePath.string());
	arch::Logger::trace("Running: {}", command);
	if (std::system(command.c_str())) {
		arch::Logger::error("Unable to perform atlas generation ('{}')", filePath.string());
		return;
	}
	arch::Logger::trace("Succesfully processed {}", filePath.string());
}

} // namespace arch::assetManager
