#pragma once

#include "Exception.h"

namespace arch {

class GLFWException final: public Exception {
public:
	GLFWException(const std::source_location& location = std::source_location::current());
};

} // namespace arch
