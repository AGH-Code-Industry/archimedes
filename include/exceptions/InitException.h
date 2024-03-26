#pragma once

#include <exception>
#include <string>

namespace arch {

class InitException: public std::exception {
public:
	explicit InitException(const std::string& message = "");

	[[nodiscard]] const char* what() const noexcept override;

private:
	std::string _message;
};

} // namespace arch
