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
	/// @param location Source location of the exception.
	///
	MathException(const std::string& message, const std::source_location& location = std::source_location::current())
	: Exception("Math", message, location) {};
};

using Exception = MathException;
} // namespace arch::math
