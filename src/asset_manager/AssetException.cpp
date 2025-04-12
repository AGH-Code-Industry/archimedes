#include <asset_manager/AssetException.h>

namespace arch::assetManager {

AssetException::AssetException(const std::string& message, const std::source_location& location):
	Exception("AssetManager", message, location) {}

}

