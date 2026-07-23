#pragma once
#include <string>

#include <archimedes/Exception.h>

namespace arch::physics {
/// @brief Exception thrown when an error occurs in the physics system.
/// @see Exception
///
class PhysicsException final: public Exception {
public:
	/// @brief Constructor.
	/// @param message Message of the exception.
	/// @param location Source location of the exception.
	///
	PhysicsException(const std::string& message, const std::stacktrace& stacktrace = std::stacktrace::current());
};

} // namespace arch::physics
