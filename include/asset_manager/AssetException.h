#pragma once

#include <string>
#include <Exception.h>

namespace arch::assetManager {

class AssetException final: public Exception {
public:
	AssetException(const std::string& message, const std::source_location& location = std::source_location::current());
};

}