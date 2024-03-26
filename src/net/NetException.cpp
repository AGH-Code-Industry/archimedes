#include "net/NetException.h"

namespace arch::net {

NetException::NetException(const std::string& title): Exception("Network submodule error: " + title) {}

} // namespace arch::net
