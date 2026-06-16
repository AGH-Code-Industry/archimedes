#include <archimedes/net/NetException.h>

namespace arch::net {

NetException::NetException(const std::string& message, const std::source_location& location):
	Exception("Network", message, location) {}

} // namespace arch::net
