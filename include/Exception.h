#pragma once

#include <exception>
#include <string>

namespace arch {

class Exception: public std::exception {
public:
	Exception(const std::string& title);
	const char* what() const noexcept override;

protected:
	void _appendMsg(const std::string& msg);

private:
	std::string _msg;
};

} // namespace arch
