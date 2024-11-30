#pragma once
#include <string>

#include <Exception.h>

namespace arch::audio {
/// @brief Exception thrown when an error occurs in the audio system.
/// @see Exception
///
class AudioException final: public Exception {
public:
	/// @brief Constructor.
	/// @param message Message of the exception.
	/// @param location Source location of the exception.
	///
	AudioException(const std::string& message, const std::source_location& location = std::source_location::current());
};

// using Exception = AudioException;
} // namespace arch::audio
