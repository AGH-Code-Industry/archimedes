#include <archimedes/net/NetException.h>

namespace arch::net {

NetException::NetException(const std::string& message, const utils::SimpleSourceLocation& loc):
	Exception("Network", message, loc) {}

} // namespace arch::net
