#include <net/exception.hpp>

namespace arch::net {
NetException::NetException(std::string title) :
	Exception("Network submodule error: " + title) {}
}