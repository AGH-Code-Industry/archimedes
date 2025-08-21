#pragma once

#include "Exception.h"

namespace arch {

/// Exception thrown when an error occurs in GLFW.
/// @see Exception
///
class GLFWException final: public Exception {
public:
	/// Constructor.
	/// @param location Source location of the exception.
	GLFWException(const std::source_location& location = std::source_location::current());
};

} // namespace arch
