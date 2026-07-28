#include <archimedes/Exception.h>

namespace arch {

Exception::Exception(std::string_view title, std::string_view message, const std::stacktrace& stacktrace):
	_title(title),
	_message(message),
	_stacktrace(stacktrace) {}

const char* Exception::what() const noexcept {
	return _message.c_str();
}

std::string_view Exception::title() const noexcept {
	return _title;
}

std::string_view Exception::message() const noexcept {
	return _message;
}

const std::stacktrace& Exception::stacktrace() const noexcept {
	return _stacktrace;
}

} // namespace arch
