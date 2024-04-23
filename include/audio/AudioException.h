#pragma once
#include <Exception.h>
#include <source_location>

namespace arch::audio {
/// @brief Exceptions that are exclusive for audio system
class AudioException: public Exception {
public:
	/// @brief Audio exceptions constructor
	/// @param description additional info related to the error
	/// @param location location of encountered exception
	AudioException(std::string_view description, const std::source_location& location = std::source_location::current());
};

using Exception = AudioException;
} // namespace arch::audio
