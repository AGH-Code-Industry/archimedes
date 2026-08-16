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
	/// @param loc location of the exception.
	///
	NetException(
		const std::string& message,
		const utils::SimpleSourceLocation& loc = utils::SimpleSourceLocation::current()
	);
};

} // namespace arch::net
