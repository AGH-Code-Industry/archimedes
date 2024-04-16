#pragma once

#include <string>

#include "Exception.h"

namespace arch {

class ConfigException final: public Exception {
public:
	ConfigException(const std::string& message, const std::source_location& location = std::source_location::current());
};

} // namespace arch
