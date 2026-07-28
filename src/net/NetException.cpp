#include <archimedes/net/NetException.h>

namespace arch::net {

NetException::NetException(const std::string& message, const std::stacktrace& stacktrace):
	Exception("Network", message, stacktrace) {}

} // namespace arch::net
