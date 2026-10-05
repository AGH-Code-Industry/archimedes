#pragma once

#include <archimedes/Exception.h>

namespace arch::font {

/// @brief Exception thrown when using fonts
class FontException: public ::arch::Exception {
public:
	/// @brief Message constructor
	/// @param msg - message of exception
	/// @param loc location of the exception.
	FontException(
		const std::string& msg,
		const utils::SimpleSourceLocation& stacktrace = utils::SimpleSourceLocation::current()
	) noexcept;
};

} // namespace arch::font
