#pragma once

#include <archimedes/Exception.h>

namespace arch::net {

/// Exception thrown when an error occurs in networking.
/// @see Exception
///
class NetException: public Exception {
public:
	/// Constructor.
	/// @param message Title of the exception.
	/// @param stacktrace Stacktrace of the exception.
	///
	NetException(const std::string& message, const std::stacktrace& stacktrace = std::stacktrace::current());
};

} // namespace arch::net
