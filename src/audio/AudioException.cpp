#include <archimedes/audio/AudioException.h>

namespace arch::audio {
AudioException::AudioException(const std::string& message, const utils::SimpleSourceLocation& loc):
	Exception("Audio", message, loc) {}
} // namespace arch::audio
