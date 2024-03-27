#include <net/NetException.h>

namespace arch::net {
NetException::NetException(std::string title) :
	Exception("Network submodule error: " + title) {}
}