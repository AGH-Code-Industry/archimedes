#pragma once
#include <Exception.h>

namespace arch::audio {
class AudioException: public Exception {
public:
	AudioException(const std::string& description);
};

using Exception = AudioException;
} // namespace arch::audio
