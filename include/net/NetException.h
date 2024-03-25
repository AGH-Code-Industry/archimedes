#include <exception.h>

namespace arch::net {
class NetException : public arch::Exception {
public:
	NetException(std::string title);
};

using Exception = NetException;
}