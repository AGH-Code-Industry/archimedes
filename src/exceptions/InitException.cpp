#include <archimedes/exceptions/InitException.h>

namespace arch {

InitException::InitException(const std::string& message, const std::stacktrace& stacktrace):
	Exception("Init", message, stacktrace) {}

} // namespace arch
