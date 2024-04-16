#pragma once

#include <string>

#include "Exception.h"

namespace arch {

class InitException final: public Exception {
public:
	InitException(const std::string& message, const std::source_location& location = std::source_location::current());
};

} // namespace arch
