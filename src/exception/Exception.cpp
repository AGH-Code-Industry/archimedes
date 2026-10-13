#include <variant>

#include <archimedes/Exception.h>

namespace arch {

Exception::Exception(std::string_view title, std::string_view message, const utils::SimpleSourceLocation& location):
	_title(title),
	_message(message),
	_locationOrStacktrace(location) {}

const char* Exception::what() const noexcept {
	return _message.c_str();
}

std::string_view Exception::title() const noexcept {
	return _title;
}

std::string_view Exception::message() const noexcept {
	return _message;
}

bool Exception::hasStacktrace() const noexcept {
	return std::holds_alternative<std::stacktrace>(_locationOrStacktrace);
}

OptRef<const utils::SimpleSourceLocation> Exception::location() const noexcept {
	if (auto locationPtr = std::get_if<utils::SimpleSourceLocation>(&_locationOrStacktrace)) {
		return *locationPtr;
	} else {
		return std::nullopt;
	}
}

OptRef<const std::stacktrace> Exception::stacktrace() const noexcept {
	if (auto stacktracePtr = std::get_if<std::stacktrace>(&_locationOrStacktrace)) {
		return *stacktracePtr;
	} else {
		return std::nullopt;
	}
}

} // namespace arch
