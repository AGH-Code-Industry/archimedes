#include <audio/exception.h>

namespace arch::audio {
AudioException::AudioException(std::string title)
	: Exception("Audio submodule error: " + title) {}
}  // namespace arch::audio