#pragma once

#include <archimedes/Exception.h>

namespace arch {

/// Exception thrown when an error occurs in GLFW.
/// @see Exception
///
class GLFWException final: public Exception {
public:
	/// Constructor.
	/// @param loc location of the exception.
	GLFWException(const utils::SimpleSourceLocation& stacktrace = utils::SimpleSourceLocation::current());
};

} // namespace arch
