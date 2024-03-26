#pragma once

#include <exception>
#include <string>

namespace arch {

class ConfigException: public std::exception {
public:
	explicit ConfigException(const std::string& message = "");

	[[nodiscard]] const char* what() const noexcept override;

private:
	std::string _message;
};

} // namespace arch
