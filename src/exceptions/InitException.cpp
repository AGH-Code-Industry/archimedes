#include "exceptions/InitException.h"

namespace arch {

InitException::InitException(const std::string& message) {
	_message = "Failed while initialization: " + message;
}

const char* InitException::what() const noexcept {
	return _message.c_str();
}

} // namespace arch
