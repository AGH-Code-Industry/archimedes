#include "net/NetException.h"

namespace arch::net {

NetException::NetException(const std::string& title, const std::source_location& location):
	Exception("Network", title, location) {}

} // namespace arch::net
