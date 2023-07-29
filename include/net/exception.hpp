#include <exception.h>

namespace arch {
class NetException : public Exception {
public:
	NetException(std::string title);
};
namespace net {
using Exception = NetException;
}
}