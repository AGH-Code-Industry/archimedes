#include <physics/PhysicsException.h>

namespace arch::physics {
PhysicsException::PhysicsException(const std::string& message, const std::source_location& location):
	Exception("Physics", message, location) {}
} // namespace arch::audio
