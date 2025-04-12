#pragma once

#include <filesystem>
#include <string>

namespace arch::assetManager {

class IAssetImporter {
public:
	virtual bool SupportsFile(const std::filesystem::path& path) const = 0;
	virtual void Import(const std::filesystem::path& sourcePath, const std::filesystem::path& processedPath) const = 0;
	virtual std::string GetType() const = 0;
	virtual ~IAssetImporter() = default;
};

} // namespace arch::assetManager
