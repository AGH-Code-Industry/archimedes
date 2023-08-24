#include "exception.h"

namespace arch {

Exception::Exception(const std::string &title) : _msg(title) {
	_msg.append(": ");
}

Exception::Exception(const std::string &title, const std::string &description) : _msg(title) {
	_msg.append(": ");
	set_description(description);
}

const char *Exception::what() const noexcept {
	return _msg.c_str();
}


void Exception::set_description(const std::string &description) {
	_msg.append(description);
}

	
}
