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
	/// @param location Source location of the exception.
	///
	InvalidEnumException(
		const std::string& message,
		const std::source_location& location = std::source_location::current()
	);
};

} // namespace arch
