#pragma once
#include <string>

#include <archimedes/Exception.h>

namespace arch::algo {
/// @brief Exception thrown when an error occurs in the algorithms system.
/// @see Exception
///
class AlgorithmException final: public Exception {
public:
public:
	/// @brief Constructor.
	/// @param message Message of the exception.
	/// @param loc location of the exception.
	///
	AlgorithmException(
		const std::string& message,
		const utils::SimpleSourceLocation& loc = utils::SimpleSourceLocation::current()
	);
};

} // namespace arch::math
