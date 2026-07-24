#pragma once

#include <archimedes/Exception.h>

namespace arch::font {

/// @brief Exception thrown when using fonts
class FontException: public ::arch::Exception {
public:
	/// @brief Message constructor
	/// @param msg - message of exception
	/// @param stacktrace Stacktrace of the exception.
	FontException(const std::string& msg, const std::stacktrace& stacktrace = std::stacktrace::current()) noexcept;
};

} // namespace arch::font
