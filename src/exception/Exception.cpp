#include <archimedes/Exception.h>

namespace arch {

Exception::Exception(std::string_view title, std::string_view message, const utils::SimpleSourceLocation& location):
	_title(title),
	_message(message),
	_location(location),
	_hasStacktrace{ false } {}

Exception::Exception(const Exception& other) {
	*this = other;
}

Exception::Exception(Exception&& other) noexcept {
	*this = std::move(other);
}

Exception& Exception::operator=(const Exception& other) {
	if (this != std::addressof(other)) {
		_title = other._title;
		_message = other._message;
		if (!other.hasStacktrace()) {
			_location = other._location;
		} else {
			_stacktrace = other._stacktrace;
		}
		_hasStacktrace = other._hasStacktrace;
	}
	return *this;
}

Exception& Exception::operator=(Exception&& other) noexcept {
	if (this != std::addressof(other)) {
		_title = std::move(other._title);
		_message = std::move(other._message);
		if (!other.hasStacktrace()) {
			_location = std::move(other._location);
		} else {
			_stacktrace = std::move(other._stacktrace);
		}
		_hasStacktrace = other._hasStacktrace;
	}
	return *this;
}

Exception::~Exception() noexcept {
	if (hasStacktrace()) {
		std::destroy_at(std::addressof(_stacktrace));
	} else {
		std::destroy_at(std::addressof(_location));
	}
}

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
	return _hasStacktrace;
}

OptRef<const utils::SimpleSourceLocation> Exception::location() const noexcept {
	if (hasStacktrace()) {
		return std::nullopt;
	} else {
		return _location;
	}
}

OptRef<const std::stacktrace> Exception::stacktrace() const noexcept {
	if (hasStacktrace()) {
		return _stacktrace;
	} else {
		return std::nullopt;
	}
}

} // namespace arch
