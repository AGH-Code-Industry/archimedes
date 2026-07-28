#include <archimedes/audio/AudioException.h>

namespace arch::audio {
AudioException::AudioException(const std::string& message, const std::stacktrace& stacktrace):
	Exception("Audio", message, stacktrace) {}
} // namespace arch::audio
