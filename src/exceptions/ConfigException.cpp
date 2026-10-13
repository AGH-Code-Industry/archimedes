#include <archimedes/exceptions/ConfigException.h>

namespace arch {

ConfigException::ConfigException(const std::string& message, const utils::SimpleSourceLocation& loc):
	Exception("Config", message, loc) {}

} // namespace arch
