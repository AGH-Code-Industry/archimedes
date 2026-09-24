#include <archimedes/math/MathException.h>

namespace arch::math {
MathException::MathException(const std::string& message, const std::stacktrace& stacktrace):
	Exception("Math", message, stacktrace) {}
} // namespace arch::math
