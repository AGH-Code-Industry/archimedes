#include "AssetException.h"

namespace arch::assetManager {

AssetException::Exception(const std::string& message, const std::source_location& location) {
	Exception("AssetManager", message, location) {}
}

}

