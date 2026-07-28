#pragma once
#include <string>

#include <archimedes/Exception.h>

namespace arch::math {
/// @brief Exception thrown when an error occurs in the math system.
/// @see Exception
///
class MathException final: public Exception {
public:
	/// @brief Constructor.
	/// @param message Message of the exception.
	/// @param stacktrace Stacktrace of the exception.
	///
	MathException(const std::string& message, const std::stacktrace& stacktrace = std::stacktrace::current());
};

} // namespace arch::math
