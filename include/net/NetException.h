#pragma once

#include "Exception.h"

namespace arch::net {

class NetException: public Exception {
public:
	NetException(const std::string& title, const std::source_location& location = std::source_location::current());
};

using Exception = NetException;

} // namespace arch::net
