#include <font/FontNotFoundException.h>

arch::font::FontNotFoundException::FontNotFoundException(
	const std::string& msg,
	const std::source_location& location
) noexcept:
	Exception("FontNotFoundException", msg, location) {}
