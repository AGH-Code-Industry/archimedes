#include <archimedes/exceptions/InvalidEnumException.h>

namespace arch {

InvalidEnumException::InvalidEnumException(const std::string& message, const std::stacktrace& stacktrace):
	Exception("InvalidEnumException", message, stacktrace) {}

} // namespace arch
