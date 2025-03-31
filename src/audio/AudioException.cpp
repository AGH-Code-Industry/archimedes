#include <audio/AudioException.h>

namespace arch::audio {
AudioException::AudioException(const std::string& message, const std::source_location& location):
	Exception("Audio", message, location) {}
} // namespace arch::audio
