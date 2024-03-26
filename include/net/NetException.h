#pragma once

#include <Exception.h>

namespace arch::net {

class NetException: public Exception {
public:
	NetException(const std::string& title);
};

using Exception = NetException;

} // namespace arch::net
