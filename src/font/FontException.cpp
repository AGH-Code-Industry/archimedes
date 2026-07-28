#include <archimedes/font/FontException.h>

arch::font::FontException::FontException(const std::string& msg, const std::stacktrace& stacktrace) noexcept:
	Exception("FontException", msg, stacktrace) {}
