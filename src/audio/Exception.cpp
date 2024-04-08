#include <audio/Exception.h>

namespace arch::audio {
AudioException::AudioException(const std::string& description)
	: Exception("Audio submodule error: " + description) {}
}  // namespace arch::audio