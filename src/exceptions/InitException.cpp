#include <archimedes/exceptions/InitException.h>

namespace arch {

InitException::InitException(const std::string& message, const utils::SimpleSourceLocation& loc):
	Exception("Init", message, loc) {}

} // namespace arch
