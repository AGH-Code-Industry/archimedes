#include <archimedes/exceptions/InitException.h>

namespace arch {

InitException::InitException(const std::string& message, const std::source_location& location):
	Exception("Init", message, location) {}

} // namespace arch
