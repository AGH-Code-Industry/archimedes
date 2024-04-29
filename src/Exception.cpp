#include "Exception.h"

namespace arch {

Exception::Exception(const std::string& title, const std::source_location& location):
	_title(title),
	_location(location) {}

Exception::Exception(const std::string& title, const std::string& message, const std::source_location& location):
	_title(title),
	_message(message),
	_location(location) {}

void Exception::print(LogLevel level) const {
	Logger::log<const std::string&, const std::string&>(level, "[{}]: {}", _title, _message, _location);
}

const char* Exception::what() const noexcept {
	return _message.c_str();
}

std::string_view Exception::title() const {
	return _title;
}

std::string_view Exception::message() const {
	return _message;
}

const std::source_location& Exception::location() const {
	return _location;
}

} // namespace arch
