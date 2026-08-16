#include <archimedes/font/FontException.h>

arch::font::FontException::FontException(const std::string& msg, const utils::SimpleSourceLocation& loc) noexcept:
	Exception("FontException", msg, loc) {}
