#pragma once

#include <exception>
#include <string>

namespace arch {

class Exception : public std::exception {
public:
	Exception(const std::string &title);
	Exception(const std::string &title, const std::string &description);
	const char *what() const noexcept override;

protected:
	void set_description(const std::string &description);

private:
	std::string _msg;
};
	
}
