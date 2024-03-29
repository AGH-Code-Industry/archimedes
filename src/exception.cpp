#include "exception.h"

namespace arch {

Exception::Exception(std::string title) : _msg(title) {
	_msg.append(": ");
}

const char *Exception::what() const noexcept {
	return _msg.c_str();
}


void Exception::append_msg(const std::string &msg) {
	_msg.append(msg);
}

}
