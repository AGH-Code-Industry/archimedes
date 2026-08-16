#pragma once

#include <string>

#include <archimedes/Exception.h>

namespace arch {

/// Exception thrown when an invalid enum value was provided.
/// @see Exception
///
class InvalidEnumException final: public Exception {
public:
	/// Constructor.
	/// @param message Message of the exception.
	/// @param loc location of the exception.
	///
	InvalidEnumException(
		const std::string& message,
		const utils::SimpleSourceLocation& loc = utils::SimpleSourceLocation::current()
	);
};

} // namespace arch
