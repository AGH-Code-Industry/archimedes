#include <archimedes/math/MathException.h>

namespace arch::math {
MathException::MathException(const std::string& message, const std::source_location& location):
	Exception("Math", message, location) {}
} // namespace arch::audio
