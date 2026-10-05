#include <archimedes/exceptions/InvalidEnumException.h>

namespace arch {

InvalidEnumException::InvalidEnumException(const std::string& message, const utils::SimpleSourceLocation& loc):
	Exception("InvalidEnumException", message, loc) {}

} // namespace arch
