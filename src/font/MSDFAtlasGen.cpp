#include <cstdio>
#include <filesystem>
#include <iostream>
#include <regex>
#include <string>

#include <font/MSDFAtlasGen.h>

namespace arch::font {

std::unique_ptr<MSDFAtlasGen> MSDFAtlasGen::_singleton;

MSDFAtlasGen& MSDFAtlasGen::get() noexcept {
	if (!_singleton) {
		_singleton = std::unique_ptr<MSDFAtlasGen>(new MSDFAtlasGen());
	}
	return *_singleton;
}

MSDFAtlasGen::MSDFAtlasGen() noexcept {
	auto cacheFile = std::ifstream("msdf_atlas_gen_cache");
	if (!cacheFile.good()) {
		_cachePath();
	} else {
		std::getline(cacheFile, _binaryPath);
		if (!std::filesystem::exists(_binaryPath)) {
			_cachePath();
		}
	}
}

void MSDFAtlasGen::_cachePath() noexcept {
	std::system("conan list msdf-atlas-gen/1.3:* > msdf_atlas_gen_temp");
	std::string packageID;
	{
		auto tempFile = std::ifstream("msdf_atlas_gen_temp");

		std::string inputFull;
		while (!tempFile.eof()) {
			std::string input;
			std::getline(tempFile, input);
			inputFull += input;
		}

		auto regex = std::regex(R"regex(packages\s*(\S.*\S)\s*info)regex");
		std::smatch matches;
		std::regex_search(inputFull, matches, regex);

		packageID = matches[1].str();
	}
	std::system((std::string("conan cache path msdf-atlas-gen/1.3:") + packageID + " > msdf_atlas_gen_temp").c_str());
	{
		auto tempFile = std::ifstream("msdf_atlas_gen_temp");
		std::string input;
		std::getline(tempFile, input);
		auto path = std::filesystem::path(input) / "bin";
		_binaryPath = std::filesystem::directory_iterator(path)->path().string();
	}
	std::filesystem::remove("msdf_atlas_gen_temp");

	std::ofstream("msdf_atlas_gen_cache") << _binaryPath;
}

std::string_view MSDFAtlasGen::path() const noexcept {
	return _binaryPath;
}

} // namespace arch::font
