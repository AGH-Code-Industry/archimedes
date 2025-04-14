#pragma once


#include <filesystem>
#include <fstream>
#include <string>
#include <cstdint>
#include <vector>
#include <map>

#include <Logger.h>

#include "IAssetImporter.h"

namespace arch::assetManager {

class MeshImporter: public IAssetImporter {
public:
	bool SupportsFile(const std::filesystem::path& path) const override;
	void Import(const std::filesystem::path& sourcePath, const std::filesystem::path& processedPath) const override;
	std::string GetType() const override;
};

} // namespace arch::assetManager

