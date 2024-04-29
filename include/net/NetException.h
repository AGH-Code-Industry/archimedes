#pragma once

#include "Exception.h"

namespace arch::net {

/// Exception thrown when an error occurs in networking.
/// @see Exception
///
class NetException: public Exception {
public:
	/// Constructor.
	/// @param message Title of the exception.
	/// @param location Source location of the exception.
	///
	NetException(const std::string& message, const std::source_location& location = std::source_location::current());
};

// Alias for ease of use.
using Exception = NetException;

} // namespace arch::net
