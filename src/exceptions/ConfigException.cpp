#include "exceptions/ConfigException.h"

namespace arch {

ConfigException::ConfigException(const std::string& message) {
	_message = "Failed load configuration file: " + message;
}

const char* ConfigException::what() const noexcept {
	return _message.c_str();
}

} // namespace arch
