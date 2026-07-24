#pragma once
#include <string>

#include <archimedes/Exception.h>

namespace arch::audio {
/// @brief Exception thrown when an error occurs in the audio system.
/// @see Exception
///
class AudioException final: public Exception {
public:
	/// @brief Constructor.
	/// @param message Message of the exception.
	/// @param stacktrace Stacktrace of the exception.
	///
	AudioException(const std::string& message, const std::stacktrace& stacktrace = std::stacktrace::current());
};

} // namespace arch::audio
