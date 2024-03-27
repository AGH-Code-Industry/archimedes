#include <exception.h>

namespace arch::audio {
class AudioException : public Exception {
public:
	AudioException(std::string title);
};

using Exception = AudioException;
}