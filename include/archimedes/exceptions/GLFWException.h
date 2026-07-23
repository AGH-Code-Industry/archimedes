#pragma once

#include <archimedes/Exception.h>

namespace arch {

/// Exception thrown when an error occurs in GLFW.
/// @see Exception
///
class GLFWException final: public Exception {
public:
	/// Constructor.
	/// @param location Source location of the exception.
	GLFWException(const std::stacktrace& stacktrace = std::stacktrace::current());
};

} // namespace arch
