#include "exceptions/InvalidEnumException.h"

namespace arch {

InvalidEnumException::InvalidEnumException(const std::string& message, const std::source_location& location):
	Exception("InvalidEnumException", message, location) {}

} // namespace arch
