#include <font/FontException.h>

arch::font::FontException::FontException(const std::string& msg, const std::source_location& location) noexcept:
	Exception("FontException", msg, location) {}
