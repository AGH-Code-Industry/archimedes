#include <audio/AudioException.h>

namespace arch::audio {
AudioException::AudioException(std::string_view description, const std::source_location& location)
	: Exception(std::string("Audio submodule error: ") + std::string(description)) {}
}  // namespace arch::audio