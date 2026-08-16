#include <archimedes/Exception.h>

namespace arch {

Exception::Exception(std::string_view title, std::string_view message, const utils::SimpleSourceLocation& location):
	_title(title),
	_message(message),
	_location(location) {}

const char* Exception::what() const noexcept {
	return _message.c_str();
}

std::string_view Exception::title() const noexcept {
	return _title;
}

std::string_view Exception::message() const noexcept {
	return _message;
}

const utils::SimpleSourceLocation& Exception::location() const noexcept {
	return _location;
}

const std::stacktrace& Exception::stacktrace() const noexcept {
	return _stacktrace;
}

} // namespace arch
