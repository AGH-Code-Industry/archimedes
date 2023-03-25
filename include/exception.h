#pragma once

#include <exception>
#include <string>

namespace arch {

class Exception : public std::exception {
public:
	Exception(std::string title);
	const char *what() const noexcept override;

protected:
	void append_msg(const std::string &msg);

private:
	std::string _msg;
};
	
}
