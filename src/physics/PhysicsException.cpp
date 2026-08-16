#include <archimedes/physics/PhysicsException.h>

namespace arch::physics {
PhysicsException::PhysicsException(const std::string& message, const utils::SimpleSourceLocation& loc):
	Exception("Physics", message, loc) {}
} // namespace arch::physics
