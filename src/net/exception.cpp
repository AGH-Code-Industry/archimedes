#include <net/exception.hpp>

namespace arch {
NetException::NetException(std::string title) :
	Exception("Network submodule error: " + title) {}
}