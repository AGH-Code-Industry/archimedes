#pragma once

#include <Exception.h>

namespace arch::font {

class FontNotFoundException: public ::arch::Exception {
public:
	FontNotFoundException(
		const std::string& msg,
		const std::source_location& location = std::source_location::current()
	) noexcept;
};

} // namespace arch::font
