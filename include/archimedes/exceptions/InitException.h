#pragma once

#include <string>

#include "Exception.h"

namespace arch {

/// Exception thrown when an error occurs during initialization.
/// @see Exception
///
class InitException final: public Exception {
public:
	/// Constructor.
	/// @param message Message of the exception.
	/// @param location Source location of the exception.
	///
	InitException(const std::string& message, const std::source_location& location = std::source_location::current());
};

} // namespace arch
