#pragma once

#include <Exception.h>

namespace arch::font {

/// @brief Exception thrown when using fonts
class FontException: public ::arch::Exception {
public:
	/// @brief Message constructor
	/// @param msg - message of exception
	/// @param location - current std::source_location
	FontException(
		const std::string& msg,
		const std::source_location& location = std::source_location::current()
	) noexcept;
};

} // namespace arch::font
