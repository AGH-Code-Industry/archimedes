#pragma once

#include <exception>
#include <source_location>

#include "Logger.h"

namespace arch {

class Exception: public std::exception {
protected:
	Exception(const std::string& title, const std::source_location& location);
	Exception(const std::string& title, const std::string& message, const std::source_location& location);

public:
	void print(LogLevel level = LogLevel::error) const;

	const char* what() const noexcept override;

	std::string_view title() const;
	std::string_view message() const;
	const std::source_location& location() const;

protected:
	std::string _title;
	std::string _message;
	std::source_location _location;
};

} // namespace arch
