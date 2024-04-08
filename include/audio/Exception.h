#pragma once
#include <Exception.h>

namespace arch::audio {
/// @brief Exceptions that are exclusive for audio system
class AudioException: public Exception {
public:
	/// @brief Audio exceptions constructor
	/// @param description additional info related to the error
	AudioException(const std::string& description);
};

using Exception = AudioException;
} // namespace arch::audio
