#include <archimedes/physics/PhysicsException.h>

namespace arch::physics {
PhysicsException::PhysicsException(const std::string& message, const std::stacktrace& stacktrace):
	Exception("Physics", message, stacktrace) {}
} // namespace arch::physics
